#ifndef X86_HPP
#define X86_HPP
#include <string>
#include <vector>
#include "symbol_table.hpp"
#include "_3AC.hpp"

using namespace std;

typedef enum instruction_types {
    I_LABEL,
    I_SEGMENT,
    I_DIRECTIVE,
    I_INSTRUCTION,
} instruction_type;

typedef enum Segment_type {
    SEG_DATA,
    SEG_TEXT,
} segment_type;

typedef struct Instruction {
    string op;
    string dst;
    string src1;
    string src2;
    instruction_type type;

    string code;

    Instruction (string op, string dst, string src1, string src2, instruction_type type);
    string make_code();
} Instruction;

typedef struct Instruction_Wrapper {
    string filename;
    vector<Instruction*> instructions;

    Instruction_Wrapper(string filename);

    void x86_init();
    void gen_x86_jump(Quadruple* quad, symbol_table* scope);
    void gen_x86_func_def(Quadruple* quad, Segment_type &prev_seg_type, symbol_table** scope);
    void gen_x86_global_decl(Quadruple* quad, Segment_type &prev_seg_type, symbol_table* scope);
    void gen_x86(vector<Quadruple*> IR, symbol_table* SYMBOL_TABLE);
} Instruction_Wrapper;

void output_x86(const string& filename, Instruction_Wrapper* x86);

#endif