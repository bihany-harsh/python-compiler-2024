#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "include/symbol_table.hpp"
using namespace std;

extern void yyerror (char const*);
char sym_error_string[256];

symbol_table* GLOBAL_SYMBOL_TABLE;

symbol_table::symbol_table() {
    
}

symbol_table::symbol_table(symbol_table_type st_type, string st_name, symbol_table* parent) {
    this->st_type = st_type;
    this->st_name = st_name;
    this->parent = parent;
}

void symbol_table::add_entry(st_entry* entry) {
    // TODO: decide the datatype for the function, class etc.
    // to add a new entry in the given symbol table
    // ERROR CASE: already present

    // TODO: proper error messages
    for (st_entry* present_entry: this->entries) {
        if (present_entry->name == entry->name) {
            snprintf(sym_error_string, sizeof(sym_error_string), "Reconsider identifier name. Already used on line no.: %d", present_entry->decl_line);
            yyerror(sym_error_string);
        }
    }
    if (this->st_type == FUNCTION) {
        // check for function parameters
        for(st_entry* param: this->xattr->func_attr.params) {
            if (param->name == entry->name) {
                yyerror("Identifier already a function parameter.");
            }
        }
    }
    if (this->st_type == CLASS) {
        for (st_entry* member: this->xattr->class_attr.members) {
            if(member->name == entry->name) {
                //NOTE: to check for STATIC POLYMORPHISM
                if (member->is_function && entry->is_function) {
                    bool flag = false;
                    for(int i = 0; i < min(member->child_symbol_table->xattr->func_attr.params.size(), entry->child_symbol_table->xattr->func_attr.params.size()); i++) {
                        st_entry* p1 = member->child_symbol_table->xattr->func_attr.params[i];
                        st_entry* p2 = entry->child_symbol_table->xattr->func_attr.params[i];
                        if (p1->b_type != p2->b_type) {
                            flag = true;
                            break;
                        }
                    }
                    if (!flag) {
                        yyerror("Function definition already exists.");
                    }
                } else {
                    yyerror("Class member already exists!");
                }
            }
        }
    }

    // ELSE push_back into the st_entry list
    this->entries.push_back(entry);
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
