#ifndef X86_HPP
#define X86_HPP
#include <string>
#include <vector>
#include <map>
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

typedef struct Function_Entry {
    string name = "";
    int offset = 0;

    Function_Entry(string name, int offset /*, base_data_type entry_type*/);
} Function_Entry;

typedef struct Function_Wrapper {
    string func_name = "";
    map <string, Function_Entry*> variable_offset_map;
    int size = 0;
    bool is_main = false;
    int num_args = 0;

    Function_Wrapper(string func_name);
    void make_function_wrapper(vector<Quadruple*> func_IR);
} Function_Wrapper;

typedef struct Instruction_Wrapper {
    string filename;

    vector<vector<Quadruple*>> functions;
    vector<Instruction*> instructions;
    vector<Instruction*> data_segment;
    vector<Function_Wrapper*> function_wrappers;

    Instruction_Wrapper(string filename);

    void gen_x86_init();
    void gen_x86(vector<Quadruple*> IR, symbol_table* SYMBOL_TABLE);

    void get_functions(vector<Quadruple*> IR);
    void gen_x86_function(vector<Quadruple*> func_IR, Function_Wrapper* func_wrapper);

    void gen_x86_basic_block(vector<Quadruple*> basic_block, Function_Wrapper* func_wrapper);
    void gen_x86_end();

    vector<Instruction*> generator(Quadruple* quad, int x, int y, int z);

    string get_func_name(string s);
} Instruction_Wrapper;

void output_x86(const string& filename, Instruction_Wrapper* x86);

// symbol_table_entry* get_func_entry(symbol_table* ST, string func_label);

bool is_main_func(string s);

bool is_init_func(string s);

bool is_variable(string name);

#endif