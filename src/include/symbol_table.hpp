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

map<base_data_type, int> base_data_type_size = {
    {INT, 4},
    {FLOAT, 8},
    {STRING, 8},
    {BOOL, 1},
    {VOID, 0}
};

typedef struct symbol_table_entry {
    string name;
    base_data_type b_type;
    int size;
    int offset;
    int decl_line;
    int scope;
    bool is_list;
    bool is_function;
    bool is_class;

    symbol_table* child_symbol_table = nullptr;

    symbol_table_entry(string name, base_data_type b_type, int size, int offset, int decl_line, int scope, bool is_list, bool is_function, bool is_class);
} st_entry;

typedef struct symbol_table {
    string st_name = "";
    vector<st_entry*> entries;
    symbol_table* parent = nullptr;
    int scope = 0;
    int offset = 0;

    symbol_table();
    symbol_table(string name, symbol_table* parent);

} symbol_table;
