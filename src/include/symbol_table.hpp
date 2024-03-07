#include <string>
#include <vector>
#include <map>
using namespace std;

typedef enum base_data_types {
    INT,
    FLOAT,
    STRING,
    BOOL,
    VOID
} base_data_type;

struct symbol_table;
struct symbol_table_entry;

typedef struct list_attributes {
    base_data_type list_elem_type;
    long long num_of_elems = 0;
} list_attr;

typedef struct func_attributes {
    vector<struct symbol_table_entry*> params;
    base_data_type return_type;
} func_attr;

typedef struct class_attributes {
    vector<struct symbol_table_entry*> members; // members as well as methods should work
} class_attr;

union xattr {
    list_attr list_attr;
    func_attr func_attr;
    class_attr class_attr;
};

map<base_data_type, int> base_data_type_size = {
    {INT, 4},
    {FLOAT, 8},
    {STRING, 8},
    {BOOL, 1},
    {VOID, 0}
};

typedef struct symbol_table_entry {
    string name = "";
    base_data_type b_type;
    int size = 0;
    int offset = 0;
    int decl_line = 0;
    int scope = 0;
    bool is_list = false;
    bool is_function = false;
    bool is_class = false;

    // additional attributes that might be required
    xattr* extended_attr = nullptr;

    symbol_table* child_symbol_table = nullptr;

    symbol_table_entry(string name, base_data_type b_type, int size, int offset, int decl_line, int scope);
    symbol_table_entry(string name, base_data_type b_type, int size, int offset, int decl_line, int scope, xattr* xattr);
} st_entry;

typedef struct symbol_table {
    string st_name = "";
    vector<st_entry*> entries;
    symbol_table* parent = nullptr;
    int scope = 0;
    int offset = 0;

    symbol_table();
    symbol_table(string st_name, symbol_table* parent);
    void add_entry(st_entry* entry);
    int delete_entry(string name);

    st_entry* get_entry(string name);
    st_entry* get_entry(string name, int scope);
    void set_scope(symbol_table* child_symbol_table); // adding a new scope

} symbol_table;
