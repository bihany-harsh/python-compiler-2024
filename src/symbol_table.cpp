#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <stack>
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
    // TODO: decide the datatype for the function, class etc.
    // to add a new entry in the given symbol table
    // ERROR CASE: already present

    // TODO: proper error messages
    for(st_entry* present_entry: this->entries) {
        if (present_entry->name == entry->name) {
            snprintf(sym_error_string, sizeof(sym_error_string), "Reconsider identifier name. Already declared on line no.: %d", present_entry->decl_line);
            yyerror(sym_error_string);
        }
    }
    // if (this->st_type == FUNCTION) {
    //     // check for function parameters
    //     for(st_entry* param: this->xattr->func_attr.params) {
    //         if (param->name == entry->name) {
    //             yyerror("Identifier already a function parameter.");
    //         }
    //     }
    // }
    // if (this->st_type == CLASS) {
    //     for (st_entry* member: this->xattr->class_attr.members) {
    //         if(member->name == entry->name) {
    //             //NOTE: to check for STATIC POLYMORPHISM
    //             if (member->is_function && entry->is_function) {
    //                 bool flag = false;
    //                 for(int i = 0; i < min(member->child_symbol_table->xattr->func_attr.params.size(), entry->child_symbol_table->xattr->func_attr.params.size()); i++) {
    //                     st_entry* p1 = member->child_symbol_table->xattr->func_attr.params[i];
    //                     st_entry* p2 = entry->child_symbol_table->xattr->func_attr.params[i];
    //                     if (p1->b_type != p2->b_type) {
    //                         flag = true;
    //                         break;
    //                     }
    //                 }
    //                 if (!flag) {
    //                     yyerror("Function definition already exists.");
    //                 }
    //             } else {
    //                 yyerror("Class member already exists!");
    //             }
    //         }
    //     }
    // }

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
        cout << "Name = " << entry->name << ", b_type = " << entry->b_type << ", size = " << entry->size << ", offset = " << entry->offset << endl;
        if(entry->child_symbol_table) {
            entry->child_symbol_table->print_st();
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

// symbol_table_entry::symbol_table_entry(string name, base_data_type b_type, int offset, int decl_line, int scope, bool is_list, bool is_function, bool is_class, extended_attr* xattr) {
//     this->name = name;
//     this->b_type = b_type;
//     this->size = base_data_type_size.find(b_type)->second;
//     this->offset = offset;
//     this->decl_line = decl_line;
//     this->scope = scope;

//     //TODO: handle for list, function, class
//     this->is_list = is_list;
//     this->is_function = is_function;
//     this->is_class = is_class;

// }

// symbol_table_entry::~symbol_table_entry() {
//     symbol_table* descendents = this->child_symbol_table;

//     while(descendents) {
//         // iterating over all descendent symbol tables
//         ~descendents;
//     }
// }

void symbol_table_entry::set_size(int size) {
    this->size = size;
}