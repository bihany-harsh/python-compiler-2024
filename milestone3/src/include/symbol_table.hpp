#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP
#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

typedef enum base_data_types {
    D_INT,
    D_FLOAT,
    D_STRING,
    D_BOOL,
    D_LIST,
    D_CLASS,
    D_FUNCTION,
    D_BLOCK,
    D_VOID,
} base_data_type;

typedef enum symbol_table_type {
    GLOBAL,
    FUNCTION,
    CLASS,
    BLOCK,
    OTHER,
} symbol_table_type;

struct symbol_table;
struct symbol_table_entry;

const map<base_data_type, string> base_data_type_map = {
    {D_BOOL, "bool"},
    {D_INT, "int"},
    {D_FLOAT, "float"},
};

typedef struct list_attributes {
    int list_elem_size = 0; // stores the size of each element of the list
    base_data_type list_elem_type = D_VOID;
    string class_name = ""; // to be used only when we have a list of classes
    long long num_of_elems = -1; // when not initialized it is -1.
} list_attr;

typedef struct func_attributes {
    int num_args = 0; // stores the number of arguments
    vector<base_data_type> args = {}; // stores the datatypes of the arguments, in order
    vector<string> list_types = {}; // stores the datatypes of the list element type, in order 
    base_data_type return_type = D_VOID; // stores the return type of the function
} func_attr;

typedef struct class_attributes {
    vector<struct symbol_table_entry*> members; // members as well as methods should work
} class_attr;

const map<base_data_type, int> base_data_type_size = {
    {D_INT, 4},
    {D_FLOAT, 8},
    {D_STRING, 8},
    {D_BOOL, 1},
    //FIXME: verify the assumption that these are all pointers to the actual data structure (runtime support) 
    {D_LIST, 8},
    {D_CLASS, 8},
    {D_FUNCTION, 8},
    {D_BLOCK, 8},
    {D_VOID, 0}
};

typedef struct symbol_table_entry {
    string name = "";
    base_data_type b_type;
    string class_name = "";
    string label = ""; // storing labels of functions
    int size = 0;
    int offset = 0;
    int decl_line = 0;
    int scope = 0;
    // NOTE: the following fields to be set explicitly by the parser.
    // These are not needed since LIST, FUNCTION, CLASS have been added to types of symbol_table_entries
    // bool is_list = false;
    // bool is_function = false;
    // bool is_class = false;

    list_attr l_attr;
    func_attr f_attr;
    class_attr c_attr;

    symbol_table* child_symbol_table = nullptr;

    // basic routines of a symbol table entry
    symbol_table_entry(string name, base_data_type b_type, int offset, int decl_line, int scope);
    // ~symbol_table_entry();
    void set_size(int size);


    // routines for symbol tables entries of a function
    void set_num_args(int num_args);
    void set_return_type(base_data_type return_type);
} st_entry;

typedef struct symbol_table {
    string st_name = "";
    string base_class_name = ""; // will be used only when the st_type is CLASS and this class is derived from some other class
    symbol_table_type st_type;
    vector<st_entry*> entries;
    symbol_table* parent = nullptr;
    int scope = 0;
    int offset = 0; // NOTE: probably not required here

    // additional attributes that might be required
    // extended_attr* xattr = nullptr;

    // basic routines for a symbol table
    symbol_table(symbol_table_type st_type, string st_name, symbol_table* parent);
    // ~symbol_table();
    void add_entry(st_entry* entry);
    st_entry* get_entry(string name);
    int delete_entry(string name);

    void print_st();

    void sort_class_entries();

    void set_scope(); // set the scope of the symbol table
    void output_st_to_csv();

} symbol_table;


void copy_func_attr(st_entry* source, st_entry* dest);

#endif