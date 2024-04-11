#include <iostream>
#include <string>
#include <vector>
#include "include/x86.hpp"
#include "include/_3AC.hpp"

using namespace std;

extern symbol_table* SYMBOL_TABLE;
extern vector<Quadruple*> IR;

vector <Instruction*> x86_code;

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
        default:
            break;
    }
    return code;
}

void Instruction_Wrapper::gen_x86(vector<Quadruple*> IR, symbol_table* SYMBOL_TABLE) {
    symbol_table* scope = SYMBOL_TABLE;
    string code = "";
    Instruction* instr;
    symbol_table_entry* st_entry;

    Segment_type prev_seg_type = SEG_TEXT;

    for (Quadruple* quad: IR) {
        if (quad->q_type == Q_ASSIGN) {
            if (scope->st_type ==  GLOBAL) {
                
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

