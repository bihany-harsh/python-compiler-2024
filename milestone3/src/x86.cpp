#include <iostream>
#include <string>
#include <vector>
#include "include/x86.hpp"
#include "include/_3AC.hpp"

using namespace std;

extern symbol_table* SYMBOL_TABLE;
extern vector<Quadruple*> IR;

int N_FUNC = 0;
string CURR_FUNC_NAME = "";

vector <Instruction*> x86_code;

symbol_table_entry* get_func_entry(symbol_table* ST, string func_label) {
    symbol_table* st = ST;
    while(st) {
        for(st_entry* entry: st->entries) {
            if(entry->b_type == D_FUNCTION && entry->label == func_label) {
                return entry;
            }
        }
        st = st->parent;
    }

    return nullptr; // could not find entry
}

void output_x86(const string& filename, Instruction_Wrapper* x86) {
    ofstream txtFile(filename);
    if (!txtFile.is_open()) {
        std::cerr << "Failed to open file for writing.\n";
        exit(1);
    }
    for (Instruction* instr: x86->instructions) {
        txtFile << instr->code << endl;
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


void Instruction_Wrapper::gen_x86_global_decl(Quadruple* quad, Segment_type &prev_seg_type, symbol_table* scope) {
    Instruction* instr;
    symbol_table_entry* st_entry;

    instr = new Instruction(".globl", quad->result, "", "", I_DIRECTIVE);
    this->instructions.push_back(instr);

    if (prev_seg_type == SEG_TEXT) {
        // in data segment
        instr = new Instruction(".data", "", "", "", I_SEGMENT);
        this->instructions.push_back(instr);
    }
    
    st_entry = scope->get_entry(quad->result);
    prev_seg_type = SEG_DATA;
    
    if (st_entry->b_type == D_INT) {
        instr = new Instruction(".align", "4", "", "", I_DIRECTIVE);
        this->instructions.push_back(instr);
        
        instr = new Instruction(".type", quad->result, "@object", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(".size", quad->result, "4", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(quad->result, "" , "", "", I_LABEL);
        this->instructions.push_back(instr);

        instr = new Instruction(".long", quad->arg1, "", "", I_DIRECTIVE);
        this->instructions.push_back(instr);
    } else if (st_entry->b_type == D_BOOL) {
        instr = new Instruction(".type", quad->result, "@object", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(".size", quad->result, "1", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(quad->result, "" , "", "", I_LABEL);
        this->instructions.push_back(instr);

        instr = new Instruction(".byte", quad->arg1 == "True" ? "1" : "0", "", "", I_DIRECTIVE);
        this->instructions.push_back(instr);
    } else if (st_entry->b_type == D_STRING) {
        int size = quad->arg1.size() - 3; // removing the \" and adding space for \0
        // quad->arg1 is of the form \"abc\" 
        string str = "\"" + quad->arg1.substr(2, quad->arg1.size() - 4) + "\"";
        if (size >= 8) {
            instr = new Instruction(".align", to_string((size > 1) ? 1 << ((sizeof(size) << 3) - __builtin_clz(size - 1)) : 0), "", "", I_DIRECTIVE);
            this->instructions.push_back(instr);
        }
        instr = new Instruction(".type", quad->result, "@object", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(".size", quad->result, to_string(size), "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(quad->result, "", "", "", I_LABEL);
        this->instructions.push_back(instr);

        instr = new Instruction(".string", str, "", "", I_DIRECTIVE);
        this->instructions.push_back(instr);
    }
}

void Instruction_Wrapper::gen_x86_func_def(Quadruple* quad, Segment_type &prev_seg_type, symbol_table** scope) {
    Instruction* instr;
    symbol_table_entry* st_entry_func;

    if (quad->arg1 == "begin_func") {
        if (prev_seg_type == SEG_DATA) {
            instr = new Instruction(".text", "", "", "", I_SEGMENT);
            this->instructions.push_back(instr);
        }

        CURR_FUNC_NAME = quad->arg2;

        instr = new Instruction(".globl", quad->arg2, "", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(".type", quad->arg2, "@function", "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        instr = new Instruction(quad->arg2, "", "", "", I_LABEL);
        this->instructions.push_back(instr);

        instr = new Instruction(".LFB" + to_string(N_FUNC), "", "", "", I_LABEL);
        this->instructions.push_back(instr);

        // push rbp
        instr = new Instruction("pushq", "%rbp", "", "", I_INSTRUCTION);
        this->instructions.push_back(instr);

        // mov rsp to rbp
        instr = new Instruction("movq", "%rsp", "%rbp", "", I_INSTRUCTION);
        this->instructions.push_back(instr);

        st_entry_func = get_func_entry(*scope, quad->arg2);
        *scope = st_entry_func->child_symbol_table;  
    } else if (quad->arg1 == "end_func") {
        // pop rbp
        instr = new Instruction("popq", "%rbp", "", "", I_INSTRUCTION);
        this->instructions.push_back(instr);

        instr = new Instruction ("ret", "", "", "", I_INSTRUCTION);
        this->instructions.push_back(instr);

        instr = new Instruction(".LFE" + to_string(N_FUNC++), "", "", "", I_LABEL);
        this->instructions.push_back(instr);

        instr = new Instruction(".size", quad->arg2, ".-" + quad->arg2, "", I_DIRECTIVE);
        this->instructions.push_back(instr);

        *scope = (*scope)->parent;

        CURR_FUNC_NAME = "";
    }
}

void Instruction_Wrapper::gen_x86_jump(Quadruple* quad, symbol_table* scope) {
    Instruction* instr;
    base_data_type return_type;
    
    if (quad->op == "return") {
        return_type = get_func_entry(scope, CURR_FUNC_NAME)->f_attr.return_type;
        if (return_type == D_INT) {
            instr = new Instruction("movq", quad->result, "%rax", "", I_INSTRUCTION);
            this->instructions.push_back(instr);
        } else if (return_type == D_BOOL) {
            instr = new Instruction("movb", quad->result, "%al", "", I_INSTRUCTION);
            this->instructions.push_back(instr);
        }
    }
}

void Instruction_Wrapper::gen_x86(vector<Quadruple*> IR, symbol_table* SYMBOL_TABLE) {
    symbol_table* scope = SYMBOL_TABLE;
    string code = "";
    Instruction* instr;
    symbol_table_entry* st_entry;

    Segment_type prev_seg_type = SEG_TEXT;

    for (Quadruple* quad: IR) {
        cout << quad->code << " and q_type " << quad->q_type << endl;
        if (quad->q_type == Q_ASSIGN) {
            if (scope->st_type ==  GLOBAL) {
                this->gen_x86_global_decl(quad, prev_seg_type, scope);
                prev_seg_type = SEG_DATA;              
            }
        } else if (quad->q_type == Q_FUNC_LABEL) {
            this->gen_x86_func_def(quad, prev_seg_type, &scope);
        } else if (quad->q_type == Q_JUMP) {
            this->gen_x86_jump(quad, scope);
        } else if (quad->q_type == Q_POP_PARAM) {
            // TODO: 
        }
    }
}

Instruction_Wrapper::Instruction_Wrapper(string filename) {
    this->filename = filename;
}

void Instruction_Wrapper::x86_init() {
    Instruction* instr = new Instruction(".text", "", "", "", I_SEGMENT);
    this->instructions.push_back(instr);
}

