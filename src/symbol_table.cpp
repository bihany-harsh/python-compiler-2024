#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "include/symbol_table.hpp"
using namespace std;

symbol_table* GLOBAL_SYMBOL_TABLE;

symbol_table::symbol_table() {
    
}

symbol_table::symbol_table(string st_name, symbol_table* parent) {
    this->st_name = st_name;
    this->parent = parent;
}

void symbol_table::add_entry(st_entry* entry) {
    
}
