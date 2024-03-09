#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP
#include <string>
#include <vector>
#include <map>
using namespace std;

typedef enum base_data_types {
    D_INT,
    D_FLOAT,
    D_STRING,
    D_BOOL,
    D_LIST_INT,
    D_LIST_FLOAT,
    D_LIST_STRING,
    D_LIST_BOOL,
    D_CLASS,
    D_FUNCTION,
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

// typedef struct list_attributes {
//     base_data_type list_elem_type;
//     long long num_of_elems = 0;
// } list_attr;

typedef struct func_attributes {
    vector<struct symbol_table_entry*> params;
    base_data_type return_type;
} func_attr;

typedef struct class_attributes {
    vector<struct symbol_table_entry*> members; // members as well as methods should work
} class_attr;

// union extended_attr {
//     func_attr func_attr;
//     class_attr class_attr;
// };

const map<base_data_type, int> base_data_type_size = {
    {D_INT, 4},
    {D_FLOAT, 8},
    {D_STRING, 8},
    {D_BOOL, 1},
    //FIXME: verify the assumption that these are all pointers to the actual data structure (runtime support) 
    {D_LIST_INT, 8},
    {D_LIST_FLOAT, 8},
    {D_LIST_STRING, 8},
    {D_LIST_BOOL, 8},
    {D_CLASS, 8},
    {D_FUNCTION, 8},
    {D_VOID, 0}
};

typedef struct symbol_table_entry {
    string name = "";
    base_data_type b_type;
    int size = 0;
    int offset = 0;
    int decl_line = 0;
    int scope = 0;
    // NOTE: the following fields to be set explicitly by the parser.
    bool is_list = false;
    bool is_function = false;
    bool is_class = false;
    // list_attr* list_attr = nullptr;

    symbol_table* child_symbol_table = nullptr;

    symbol_table_entry(string name, base_data_type b_type, int offset, int decl_line, int scope);
    void set_size(int size);
    // symbol_table_entry(string name, base_data_type b_type, int offset, int decl_line, int scope, bool is_list, bool is_function, bool is_class, extended_attr* xattr);
    // ~symbol_table_entry();
} st_entry;

typedef struct symbol_table {
    symbol_table_type st_type;
    string st_name = "";
    vector<st_entry*> entries;
    symbol_table* parent = nullptr;
    int scope = 0;
    int offset = 0; // NOTE: probably not required here

    // additional attributes that might be required
    // extended_attr* xattr = nullptr;

    symbol_table(symbol_table_type st_type, string st_name, symbol_table* parent);
    void add_entry(st_entry* entry);
    int delete_entry(string name);

    st_entry* get_entry(string name);
    void set_scope(); // adding a new scope
    void print_st();
} symbol_table;
#endif