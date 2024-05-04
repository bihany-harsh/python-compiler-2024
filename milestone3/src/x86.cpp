#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "include/x86.hpp"
#include "include/_3AC.hpp"

using namespace std;

extern symbol_table* SYMBOL_TABLE;
extern vector<Quadruple*> IR;

extern void yyerror(const char*);

map<string, string> func_name_map;
int func_count = 0;
bool allocmem_present = false, print_present = false, print_str_present = false;

bool is_variable(string s) {
    return !(s[0] >= '0' && s[0] <= '9') && (s[0] != '-') && (s[0] != '+');
}

bool is_string_literal(string s) {
    return s[0] == '"';
}

string Instruction_Wrapper::get_func_name(string s) {
    if(func_name_map.find(s) == func_name_map.end()) {
        func_count++;
        func_name_map[s] = "func" + to_string(func_count);
    }

    return func_name_map[s];
}

void output_x86(const string& filename, Instruction_Wrapper* x86) {
    ofstream txtFile(filename);
    string line;
    if (!txtFile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        exit(1);
    }
    for (Instruction* instr: x86->data_segment) {
        txtFile << instr->code << endl;
    }
    for (Instruction* instr: x86->instructions) {
        txtFile << instr->code << endl;
    }

    if (allocmem_present) {
        ifstream alloc_mem("allocmem.s");
        while(getline(alloc_mem, line)) {
            txtFile << line << '\n';
        }
    }

    if (print_present) {
        ifstream print_func("print.s");
        while(getline(print_func, line)){
            txtFile << line << '\n';
        }
    }

    if (print_str_present) {
        ifstream print_func_str("printstr.s");
        while(getline(print_func_str, line)) {
            txtFile << line << '\n';
        }
    }
}

Instruction::Instruction(string op, string dst, string src1, string src2, instruction_type type) {
    this->op = op;
    this->dst = dst;
    this->src1 = src1;
    this->src2 = src2;
    this->type = type;

    this->code = this->make_code();   
}

string Instruction::make_code() {
    string code = "";
    switch(this->type) {
        case I_LABEL:
            code = this->op + ":";
            break;
        case I_SEGMENT:
            code = "\t" + this->op;
            break;
        case I_DIRECTIVE:
            code = "\t" + this->op;
            if (this->dst != "") {
                code += " " + this->dst;
                if (this->src1 != "") {
                    code += ", " + this->src1;
                    if (this->src2 != "") {
                        code += ", " + this->src2;
                    }
                }
            } else {
                code = "\t" + this->op;
            }
            break;
        case I_INSTRUCTION:
            code = "\t" + this->op;
            if (this->dst != "") {
                code += " " + this->dst;
                if (this->src1 != "") {
                    code += ", " + this->src1;
                    if (this->src2 != "") {
                        code += ", " + this->src2;
                    }
                }
            }
        default:
            break;
    }
    return code;
}

bool is_main_func(string s) {
    return s == "main";
}

bool is_init_func(string str) {
    std::string pattern = "$__init__";
    size_t pos = str.find(pattern);
    
    if (pos != string::npos && pos > 0) {
        return true;
    }
    return false;
}

Function_Entry::Function_Entry(string name, int offset /*, base_data_type b_type*/) {
    this->name = name;
    this->offset = offset;
}

Function_Wrapper::Function_Wrapper(string func_name) {
    this->func_name = func_name;
}

void Function_Wrapper::make_function_wrapper(vector<Quadruple*> func_IR) {
    int pop_cnt = 2; // `pushq rbp` and `return address` 
    int local_offset = 8;   // 8 callee saved registers

    for (Quadruple* quad: func_IR) {
        if ((quad->q_type == Q_FUNC_LABEL && quad->arg1 == "begin_func") || quad->q_type == Q_SP_UPDATE) {
            continue;
        }
        if (quad->q_type == Q_FUNC_CALL) {
            if (this->variable_offset_map.find(quad->result) == this->variable_offset_map.end()) {
                this->variable_offset_map[quad->result] = new Function_Entry(quad->result, -8*local_offset);
                local_offset++;
            }
        }
        if (quad->q_type == Q_POP_PARAM) {
            this->variable_offset_map[quad->result] = new Function_Entry(quad->result, 8*pop_cnt);
            pop_cnt++;
        } else {
            if (quad->q_type == Q_COND_JUMP) {
                if (this->variable_offset_map.find(quad->arg1) == this->variable_offset_map.end()) {
                    this->variable_offset_map[quad->arg1] = new Function_Entry(quad->arg1, -8*local_offset);
                    local_offset++;
                } 
            } else if (quad->q_type == Q_JUMP && quad->op == "goto") {
                continue;
            } else {
                if (quad->arg1 != "" && (this->variable_offset_map.find(quad->arg1) == this->variable_offset_map.end()) && is_variable(quad->arg1)) {
                    this->variable_offset_map[quad->arg1] = new Function_Entry(quad->arg1, -8*local_offset);
                    local_offset++;
                } 
                if (quad->arg2 != "" && (this->variable_offset_map.find(quad->arg2) == this->variable_offset_map.end()) && is_variable(quad->arg2)) {
                    this->variable_offset_map[quad->arg2] = new Function_Entry(quad->arg2, -8*local_offset);
                    local_offset++;
                }
                if (quad->result != "" && (this->variable_offset_map.find(quad->result) == this->variable_offset_map.end()) && is_variable(quad->result)) {
                    this->variable_offset_map[quad->result] = new Function_Entry(quad->result, -8*local_offset);
                    local_offset++;
                }
            }
        }
    }

    this->size = 8*local_offset;
}

Instruction_Wrapper::Instruction_Wrapper(string filename) {
    this->filename = filename;   
}

vector<Instruction*> Instruction_Wrapper::generator(Quadruple* quad, int x, int y, int z) {
    vector<Instruction*> instructions;
    Instruction* instr;

    if (quad->code == "") {
        return instructions;
    }
    if (quad->is_target) {
        instr = new Instruction("L" + quad->label, "", "", "", I_LABEL);
        instructions.push_back(instr);
    }

    if (quad->q_type == Q_BINARY) {
        if (quad->op == "+") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("addq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("addq", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "-") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("subq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("subq", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "*") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("imulq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("imulq", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "//" || quad->op == "/") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rax", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rax", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cqto", "", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rbx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rbx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("idivq", "%rbx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("movq", "%rax", "%rdx", "", I_INSTRUCTION);
        } else if (quad->op == "%") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rax", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rax", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cqto", "", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rbx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rbx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("idivq", "%rbx", "", "", I_INSTRUCTION);
        } else if (quad->op == "**") {
            // power function
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if(!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rax", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("testq", "%rcx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jz", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("imulq", "%rdx", "%rax", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("decq", "%rcx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jnz", "1b", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "%rax", "%rdx", "", I_INSTRUCTION);
        } else if (quad->op == ">>") {
            if (is_variable(quad->arg1)) {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("sarq", "%cl", "%rdx", "", I_INSTRUCTION);
        } else if (quad->op == "<<") {
            if (is_variable(quad->arg1)) {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("salq", "%cl", "%rdx", "", I_INSTRUCTION);
        } else if (quad->op == "and") {
            // cout << quad->code << endl;
            if (is_variable(quad->arg1)) {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("and", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("and", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "or") {
            if (is_variable(quad->arg1)) {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("or", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("or", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == ">") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jl", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "<") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jg", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if(quad->op == ">=") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jle", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "<=") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jge", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "==") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("je", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "!=") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rcx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jne", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "&") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("and", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("and", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "|") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("or", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("or", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "^") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("xor", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("xor", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "&&") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("je", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("je", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "||") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jne", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            if (!is_variable(quad->arg2)) {
                instr = new Instruction("movq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("cmpq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jne", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        }

        instructions.push_back(instr);

        instr = new Instruction("movq", "%rdx", to_string(z) + "(%rbp)", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_BINARY_STR) {
        instr = new Instruction("pushq", "%rbp", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", "%rsp", "%rbp", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rbx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rdi", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rsi", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r12", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r13", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r14", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r15", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdi", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rsi", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("call", "strcmp@PLT", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        // rax has result

        instr = new Instruction("movq", "%rax", "%rdx", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", "$0", "%rcx", "", I_INSTRUCTION);
        instructions.push_back(instr);

        if (quad->op == ">") {
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jl", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "<") {
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jg", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == ">=") {
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jle", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "<=") {
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jge", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "==") {
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("je", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        } else if (quad->op == "!=") {
            instr = new Instruction("cmpq", "%rdx", "%rcx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jne", "1f", "", "", I_INSTRUCTION); // TRUE
            instructions.push_back(instr);
            instr = new Instruction("movq", "$0", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION); // FALSE
            instructions.push_back(instr);
            instr = new Instruction("1", "", "", "", I_LABEL);
            instructions.push_back(instr);
            instr = new Instruction("movq", "$1", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("jmp", "2f", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("2", "", "", "", I_LABEL);
        }

        instructions.push_back(instr);

        instr = new Instruction("movq", "%rdx", to_string(z) + "(%rbp)", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_UNARY) {
        if (quad->op == "~") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION); 
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("neq", "%rdx", "", "", I_INSTRUCTION);
        } else if (quad->op == "not") {
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION); 
            } else {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
            instr = new Instruction("not", "%rdx", "", "", I_INSTRUCTION);
        } else if (quad->op == "-") {
            instr = new Instruction("xor", "%rdx", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("subq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION); 
            } else {
                instr = new Instruction("subq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        } else if (quad->op == "+") {
            instr = new Instruction("xor", "%rdx", "%rdx", "", I_INSTRUCTION);
            instructions.push_back(instr);
            if (!is_variable(quad->arg1)) {
                instr = new Instruction("addq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION); 
            } else {
                instr = new Instruction("addq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
            }
        }

        instructions.push_back(instr);
        instr = new Instruction("movq", "%rdx", to_string(y) + "(%rbp)", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_ASSIGN) {
        if (!is_variable(quad->arg1)) {
            instr = new Instruction("movq", "$" + quad->arg1, to_string(y) + "(%rbp)", "", I_INSTRUCTION);
            instructions.push_back(instr); 
        } else {
            if (is_string_literal(quad->arg1)) {
                instr = new Instruction(quad->result + "_str:", ".asciz", quad->arg1, "", I_INSTRUCTION);
                this->data_segment.push_back(instr);

                instr = new Instruction("leaq", quad->result + "_str(%rip)", "%rdx", "", I_INSTRUCTION);
                instructions.push_back(instr);

                instr = new Instruction("movq", "%rdx", to_string(y) + "(%rbp)", "", I_INSTRUCTION);
                instructions.push_back(instr);
            } else  {
                instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
                instructions.push_back(instr);
                instr = new Instruction("movq", "%rdx", to_string(y) + "(%rbp)", "", I_INSTRUCTION);
                instructions.push_back(instr);
            }
        }
    } else if (quad->q_type == Q_COND_JUMP) {
        if (!is_variable(quad->arg1)) {
            instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
        } else {
            instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
        }
        instructions.push_back(instr);
        instr = new Instruction("cmpq", "$0", "%rdx", "", I_INSTRUCTION);
        instructions.push_back(instr);

        // we have ifFalse semantic
        instr = new Instruction("je", "L" + to_string(y), "", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_JUMP && quad->op == "goto") {
        instr = new Instruction("jmp", "L" + to_string(x), "", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_STORE) {
        if (!is_variable(quad->arg1)) {
            instr = new Instruction("movq", "$" + quad->arg1, "%rax", "", I_INSTRUCTION);
        } else {
            instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rax", "", I_INSTRUCTION);
        }
        instructions.push_back(instr);
        instr = new Instruction("movq", to_string(z) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", "%rax", "(%rdx)", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_DEREFERENCE) {
        instr = new Instruction("movq", to_string(x) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", "(%rdx)", "%rdx", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", "%rdx", to_string(z) + "(%rbp)", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_FUNC_LABEL && quad->arg1 == "begin_func") {
        if (y) {
            // main function
            instr = new Instruction("main", "", "", "", I_LABEL);
            instructions.push_back(instr);
        }
        instr = new Instruction(get_func_name(quad->arg2), "", "", "", I_LABEL);
        instructions.push_back(instr);

        instr = new Instruction("pushq", "%rbp", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("movq", "%rsp", "%rbp", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rbx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rdi", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rsi", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r12", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r13", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r14", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r15", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        if (x > 0) {
            instr = new Instruction("subq", "$" + to_string(x), "%rsp", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }
    } else if (quad->q_type == Q_JUMP && quad->op == "return") {
        if (quad->result != "") {
            if (!is_variable(quad->result)) {
                instr = new Instruction("movq", "$" + quad->result, "%rax", "", I_INSTRUCTION);
            } else {
                instr = new Instruction("movq", to_string(y) + "(%rbp)", "%rax", "", I_INSTRUCTION);
            }
            instructions.push_back(instr);
        }


        instr = new Instruction("addq", "$" + to_string(x), "%rsp", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r15", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r14", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r13", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r12", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rsi", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rdi", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rbx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rbp", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("ret", "", "", "", I_INSTRUCTION);
        instructions.push_back(instr);        
    } else if (quad->q_type == Q_FUNC_LABEL && quad->arg1 == "end_func") {
        if (x == 1) {
            // main function
            instr = new Instruction("movq", "$60", "%rax", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("xor", "%rdi", "%rdi", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("syscall", "", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
        } else {
            // if *$__init__$
            if (is_init_func(quad->arg2)) {
                instr = new Instruction("movq", "16(%rbp)", "%rax", "", I_INSTRUCTION);
                instructions.push_back(instr);
            }

            instr = new Instruction("addq", "$" + to_string(y), "%rsp", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%r15", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%r14", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%r13", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%r12", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%rsi", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%rdi", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%rbx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("popq", "%rbp", "", "", I_INSTRUCTION);
            instructions.push_back(instr);

            instr = new Instruction("ret", "", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }
    } else if (quad->q_type == Q_SP_UPDATE) {

    } else if (quad->q_type == Q_FUNC_CALL) {

        if (x == 0) {
            instr = new Instruction("pushq", "%rax", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%rcx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%rdx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r8", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r9", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r10", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r11", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }

        instr = new Instruction("call", this->get_func_name(quad->arg1), "", "", I_INSTRUCTION);
        instructions.push_back(instr);


        if (x > 0) {
            instr = new Instruction("addq", "$" + to_string(x*8), "%rsp", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }

        if (quad->result != "") {
            instr = new Instruction("movq", "%rax", to_string(y) + "(%rbp)", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }

        instr = new Instruction("popq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

    } else if (quad->q_type == Q_PRINT) {
        instr = new Instruction("pushq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        if (!is_variable(quad->arg1)) {
            instr = new Instruction("pushq", "$" + quad->arg1, "", "", I_INSTRUCTION);
        } else {
            instr = new Instruction("pushq", to_string(x) + "(%rbp)", "", "", I_INSTRUCTION);
        }

        instructions.push_back(instr);

        instr = new Instruction("call", "print", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("addq", "$8", "%rsp", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("popq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if(quad->q_type == Q_PRINT_STR) {
        instr = new Instruction("pushq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("pushq", to_string(x) + "(%rbp)", "", "", I_INSTRUCTION);

        instructions.push_back(instr);

        instr = new Instruction("call", "printstr", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("addq", "$8", "%rsp", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("popq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_ALLOC) {
        instr = new Instruction("pushq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("pushq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        if (!is_variable(quad->arg1)) {
            instr = new Instruction("pushq", "$" + quad->arg1, "", "", I_INSTRUCTION);
        } else {
            instr = new Instruction("pushq", to_string(x) + "(%rbp)", "", "", I_INSTRUCTION);
        }

        instructions.push_back(instr);

        instr = new Instruction("call", "allocmem", "", "", I_INSTRUCTION);
        instructions.push_back(instr);

        instr = new Instruction("addq", "$8", "%rsp", "", I_INSTRUCTION);
        instructions.push_back(instr);

        if (quad->result != "") {
            instr = new Instruction("movq", "%rax", to_string(z) + "(%rbp)", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }

        instr = new Instruction("popq", "%r11", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r10", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r9", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%r8", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rdx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rcx", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
        instr = new Instruction("popq", "%rax", "", "", I_INSTRUCTION);
        instructions.push_back(instr);
    } else if (quad->q_type == Q_PUSH_PARAM) {
        if (y == 1) {
            // first parameter passed
            instr = new Instruction("pushq", "%rax", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%rcx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%rdx", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r8", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r9", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r10", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
            instr = new Instruction("pushq", "%r11", "", "", I_INSTRUCTION);
            instructions.push_back(instr);
        }
        if (!is_variable(quad->result)) {
            instr = new Instruction("pushq", "$" + quad->result, "", "", I_INSTRUCTION);
        } else {
            // cout << "PUSH_PARAM " << x << endl; 
            instr = new Instruction("pushq", to_string(x) + "(%rbp)", "", "", I_INSTRUCTION);
        }
        instructions.push_back(instr);
    }


    return instructions;
}

void Instruction_Wrapper::gen_x86_basic_block(vector<Quadruple*> basic_block, Function_Wrapper* func_wrapper) {
    // TODO: add support for Coercion
    for(Quadruple* quad: basic_block) {
        vector<Instruction*> gen_instruction;
        int x, y, z;

        if (quad->q_type == Q_COND_JUMP) {
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            y = atoi(quad->result.c_str());
            gen_instruction = this->generator(quad, x, y, -1);
        } else if (quad->q_type == Q_JUMP && quad->op == "goto") {
            gen_instruction = this->generator(quad, atoi(quad->result.c_str()), -1, -1);
        } else if (quad->q_type == Q_BINARY) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                z = -1;
            } else {
                z = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg2) == func_wrapper->variable_offset_map.end()) {
                y = -1;
            } else {
                y = func_wrapper->variable_offset_map[quad->arg2]->offset;
            }
            gen_instruction = this->generator(quad, x, y, z);
        } else if (quad->q_type == Q_UNARY) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                y = -1;
            } else {
                y = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            gen_instruction = this->generator(quad, x, y, -1);
        } else if (quad->q_type == Q_ASSIGN) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                y = -1;
            } else {
                y = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            gen_instruction = this->generator(quad, x, y, -1);
        } else if (quad->q_type == Q_STORE) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                z = -1;
            } else {
                z = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            gen_instruction = this->generator(quad, x, -1, z);
        } else if (quad->q_type == Q_DEREFERENCE) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                z = -1;
            } else {
                z = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            gen_instruction = this->generator(quad, x, -1, z);
        } else if (quad->q_type == Q_PUSH_PARAM) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            func_wrapper->num_args++;
            gen_instruction = this->generator(quad, x, func_wrapper->num_args, -1);
        } else if (quad->q_type == Q_POP_PARAM) {
            gen_instruction = this->generator(quad, -1, -1, -1);
        } else if (quad->q_type == Q_PRINT) {
            print_present = true;
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            gen_instruction = this->generator(quad, x, -1, -1);
        } else if (quad->q_type == Q_PRINT_STR) {
            print_str_present = true;
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            gen_instruction = this->generator(quad, x, -1, -1);
        } else if (quad->q_type == Q_ALLOC) {
            allocmem_present = true;
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                z = -1;
            } else {
                z = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            gen_instruction = this->generator(quad, x, -1, z);
        } else if (quad->q_type == Q_FUNC_CALL) {
            gen_instruction = this->generator(quad, func_wrapper->num_args, func_wrapper->variable_offset_map[quad->result]->offset, -1);
            func_wrapper->num_args = 0;
        } else if (quad->q_type == Q_FUNC_LABEL && quad->arg1 == "begin_func") {
            if (is_main_func(quad->arg2)) {
                func_wrapper->is_main = true;
            }
            gen_instruction = this->generator(quad, func_wrapper->size - 8*8, (int)func_wrapper->is_main, -1);
        } else if (quad->q_type == Q_JUMP && quad->op == "return") {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                y = -1;
            } else {
                y = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            gen_instruction = this->generator(quad, func_wrapper->size - 8*8, y, -1);
        } else if (quad->q_type == Q_FUNC_LABEL && quad->arg1 == "end_func") {
            gen_instruction = this->generator(quad, (int)func_wrapper->is_main, func_wrapper->size - 8*8, -1);
        } else if (quad->q_type == Q_SP_UPDATE) {
            gen_instruction = this->generator(quad, -1, -1, -1);
        } else if (quad->q_type == Q_BINARY_STR) {
            if (func_wrapper->variable_offset_map.find(quad->result) == func_wrapper->variable_offset_map.end()) {
                z = -1;
            } else {
                z = func_wrapper->variable_offset_map[quad->result]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg1) == func_wrapper->variable_offset_map.end()) {
                x = -1;
            } else {
                x = func_wrapper->variable_offset_map[quad->arg1]->offset;
            }
            if (func_wrapper->variable_offset_map.find(quad->arg2) == func_wrapper->variable_offset_map.end()) {
                y = -1;
            } else {
                y = func_wrapper->variable_offset_map[quad->arg2]->offset;
            }
            gen_instruction = this->generator(quad, x, y, z);
        } else {
            gen_instruction = this->generator(quad, -1, -1, -1);
        }

        for (Instruction* instr: gen_instruction) {
            this->instructions.push_back(instr);
        }
    }
}

void Instruction_Wrapper::gen_x86_function(vector<Quadruple*> func_IR, Function_Wrapper* func_wrapper) {
    set<int> leaders;
    vector<Quadruple*> basic_block;
    vector<int> ascending_leaders;
    int prev_leader = 0;

    int base_offset = atoi(func_IR[0]->label.c_str());

    leaders.insert(base_offset);
    
    for(Quadruple* quad: func_IR) {
        if (quad->q_type == Q_COND_JUMP || (quad->q_type == Q_JUMP && quad->op == "goto")) {
            leaders.insert(atoi(quad->result.c_str()));
            leaders.insert(atoi(quad->label.c_str()) + 1);
        } else if (quad->q_type == Q_FUNC_CALL) {
            leaders.insert(atoi(quad->label.c_str()));
            leaders.insert(atoi(quad->label.c_str()) + 1);
        }
    }

    for(int leader : leaders) { 
        ascending_leaders.push_back(leader); 
    }

    prev_leader = ascending_leaders[0];

    for(int i = 1; i < ascending_leaders.size(); i++) {
        basic_block.clear();
        
        for(int j = prev_leader; j < ascending_leaders[i]; j++) {
            basic_block.push_back(func_IR[j - base_offset]);
        }
        prev_leader = ascending_leaders[i];

        this->gen_x86_basic_block(basic_block, func_wrapper);
    }

    basic_block.clear();
    int final_leader = ascending_leaders[ascending_leaders.size() - 1];
    for(int i = final_leader; i - base_offset < func_IR.size(); i++) {
        basic_block.push_back(func_IR[i - base_offset]);
    }

    this->gen_x86_basic_block(basic_block, func_wrapper);
}

void Instruction_Wrapper::gen_x86_init() {
    Instruction* instr;

    instr = new Instruction(".data", "", "", "", I_SEGMENT);
    this->data_segment.push_back(instr);

    instr = new Instruction("string_format:", ".asciz", "\"%s\\n\"", "", I_INSTRUCTION);
    this->data_segment.push_back(instr);

    instr = new Instruction("integer_format:", ".asciz", "\"%ld\\n\"", "", I_INSTRUCTION);
    this->data_segment.push_back(instr);

    instr = new Instruction(".globl", "main", "", "", I_DIRECTIVE);
    this->instructions.push_back(instr);
}

void Instruction_Wrapper::gen_x86(vector<Quadruple*> IR, symbol_table* SYMBOL_TABLE) {
    Instruction* instr;
    Function_Wrapper* function_wrapper;

    instr = new Instruction(".text", "", "", "", I_SEGMENT);
    this->instructions.push_back(instr);

    this->get_functions(IR);

    for (vector<Quadruple*> function: this->functions) {
        function_wrapper = new Function_Wrapper(function[0]->arg2); // first quad is `begin_func func_name`
        function_wrapper->make_function_wrapper(function);

        this->function_wrappers.push_back(function_wrapper);
        this->gen_x86_function(function, function_wrapper);
    }

}

void Instruction_Wrapper::get_functions(vector<Quadruple*> IR) {
    vector<Quadruple*> function;

    bool func_started = false;

    for (Quadruple* quad: IR) {
        if (quad->q_type == Q_FUNC_LABEL && quad->arg1 == "begin_func") {
            func_started = true;
        } 
        if (func_started) {
            function.push_back(quad);
        }
        if (quad->q_type == Q_FUNC_LABEL && quad->arg1 == "end_func") {
            func_started = false;
            if (function.size() > 0) {
                this->functions.push_back(function);
                function.clear();
            }
        }
    }
}

