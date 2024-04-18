#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <stack>
#include <fstream>
#include "include/symbol_table.hpp"
using namespace std;


extern void yyerror (char const*);
char sym_error_string[256];

symbol_table* SYMBOL_TABLE;
stack<symbol_table*> ST_STACK;
st_entry* dummy_entry;

stack<int> OFFSET_STACK;
int OFFSET = 0;
int block_counter = 0; // keeps a track of how many block scopes have been created (for giving unique names to st_entries of each block)
int num_args = 0; // for function parameters, is updated in pyparse.y

ofstream csvFile;

symbol_table::symbol_table(symbol_table_type st_type, string st_name, symbol_table* parent) {
    this->st_type = st_type;
    this->st_name = st_name;
    this->parent = parent;
    if(parent) {
        this->scope = parent->scope + 1;
    }
    else {
        this->scope = 0; // when creating the first table
    }
}

void symbol_table::add_entry(st_entry* entry) {
    // to add a new entry in the given symbol table
    // ERROR CASE: entry already present

    for(st_entry* present_entry: this->entries) {
        if (present_entry->name == entry->name) {
            if(entry->b_type == D_FUNCTION && present_entry->b_type == D_FUNCTION) {
                // any other type of clash (var with class/ func with class/ class with class) is not allowed
                // If a function, check only arguments, needn't check return type for static polymorphism
                if(entry->f_attr.num_args == present_entry->f_attr.num_args) {
                    bool args_diff = false;
                    for(int i = 0; i < present_entry->f_attr.num_args; i++) {
                        if(present_entry->f_attr.args[i] != entry->f_attr.args[i]) {
                            args_diff = true;
                            break;
                        }
                    }
                    if(!args_diff) {
                        yyerror(("Function with same prototype already declared on line no.: " + to_string(present_entry->decl_line)).c_str());
                    }
                }
            }
            else {
                yyerror(("Reconsider identifier name " + entry->name + ". Already declared on line no.:" + to_string(present_entry->decl_line)).c_str());
            }
        }
    }

    // ELSE push_back into the st_entry list
    this->entries.push_back(entry);
    this->offset + entry->size;
}

int symbol_table::delete_entry(string name) {
    for (int i = 0; i < this->entries.size(); i++) {
        if(this->entries[i]->name == name) {
            delete this->entries[i];
            auto newEnd = remove(this->entries.begin(), this->entries.end(), nullptr);
            this->entries.erase(newEnd, this->entries.end());
            return 0;
        }
    }

    return -1; // erroneous!
}

void symbol_table::print_st() {
    for(st_entry* entry: this->entries) {
        if(entry->b_type == D_FUNCTION) {
            cout << "Name = " << entry->name << ", b_type = " << entry->b_type << ", size = " << entry->size << ", num_args = " << entry->f_attr.num_args << ", return_type = " << entry->f_attr.return_type << ", offset = " << entry->label << endl;
        }
        else if(entry->b_type == D_LIST) {
            cout << "Name = " << entry->name << ", b_type = " << entry->b_type << ", size = " << entry->size << ", offset = " << entry->offset << ", num_elements = " << entry->l_attr.num_of_elems << ", element_type = " << entry->l_attr.list_elem_type << " , list elem size = " << entry->l_attr.list_elem_size << endl;
        }
        else {
            cout << "Name = " << entry->name << ", b_type = " << entry->b_type << ", size = " << entry->size << ", offset = " << entry->offset << endl;
        }
        if(entry->child_symbol_table) {
            cout << "Entering the child symbol table" << endl;
            entry->child_symbol_table->print_st();
            cout << "Returned from the child symbol table" << endl;
        }
    }
}

void symbol_table::output_st_to_csv() {
    if(!csvFile.is_open()) {
        cerr << "Failed to open csv file for writing.\n";
        exit(1);
    }

    for(st_entry* entry: this->entries) {
        csvFile << entry->name << "," << entry->b_type << "," << entry->size << "," << entry->offset << "," << entry->decl_line << "," << entry->scope << endl;
        if(entry->child_symbol_table) {
            entry->child_symbol_table->output_st_to_csv();
        }
    }
}

st_entry* symbol_table::get_entry(string name) {
    symbol_table* st = this;
    while(st) {
        for(st_entry* entry: st->entries) {
            if(entry->name == name) {
                return entry;
            }
        }
        st = st->parent;
    }

    return nullptr; // could not find entry
}

void symbol_table::set_scope() {
    this->scope = this->parent->scope + 1;
}

symbol_table_entry::symbol_table_entry(string name, base_data_type b_type, int offset, int decl_line, int scope) {
    this->name = name;
    this->b_type = b_type;
    this->size = base_data_type_size.find(b_type)->second;
    this->offset = offset;
    this->decl_line = decl_line;
    this->scope = scope;
}

void symbol_table_entry::set_size(int size) {
    if (this->b_type == D_CLASS) {
        size = 0;
        for (st_entry* entry: this->child_symbol_table->entries) {
            if (entry->b_type != D_FUNCTION) {
                size += entry->size;
            }
        }
        this->size = size;
    } else {
        this->size = size;
    }
}

void symbol_table_entry::set_num_args(int num_args) {
    if(this->b_type != D_FUNCTION) {
        cout << "Erroneous call to the function set_arg_types" << endl;
        exit(1);
    }
    this->f_attr.num_args = num_args;
}

void symbol_table_entry::set_return_type(base_data_type return_type) {
    if(this->b_type != D_FUNCTION) {
        cout << "Erroneous call to the function set_return_type" << endl;
        exit(1);
    }
    this->f_attr.return_type = return_type;
}

void symbol_table::sort_class_entries() {
    if (this->st_type != CLASS) {
        yyerror("Unexpected Error: Erroneous call to the function sort_class_entries.");
    }
    
    stable_partition(this->entries.begin(), this->entries.end(),
        [](const st_entry* entry) {
            return entry->b_type != D_FUNCTION;
        }
    );
    int offset = 0;
    for (st_entry* entry : this->entries) {
        entry->offset = offset;
        offset += entry->size;
    }
}

void copy_func_attr(st_entry* src, st_entry* dest) {
    dest->label = src->label;
    dest->size = src->size;
    dest->f_attr.num_args = src->f_attr.num_args;
    dest->f_attr.return_type = src->f_attr.return_type;
    for(int i = 0; i < src->f_attr.num_args; i++) {
        dest->f_attr.args.push_back(src->f_attr.args[i]);
        dest->f_attr.list_types.push_back(src->f_attr.list_types[i]);
    }
    dest->child_symbol_table = src->child_symbol_table;
    return;
}

void set_return_list_attr(st_entry* entry, string data_type) {
    if(data_type == "int") {
        entry->f_attr.return_list_attr.list_elem_type = D_INT;
    }
    else if(data_type == "str") {
        entry->f_attr.return_list_attr.list_elem_type = D_STRING;
    }
    else if(data_type == "float") {
        entry->f_attr.return_list_attr.list_elem_type = D_FLOAT;
    }
    else if(data_type == "bool") {
        entry->f_attr.return_list_attr.list_elem_type = D_BOOL;
    }
    else {
        entry->f_attr.return_list_attr.list_elem_type = D_CLASS;
        entry->f_attr.return_list_attr.class_name = data_type;
    }
}