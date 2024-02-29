#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "include/node.hpp"
using namespace std;

long long int NODE_COUNT = 0;

node* AST_ROOT = NULL;

extern int yylineno;
extern ostringstream dot_stream;

node::node(node_type type, string name, bool is_terminal, node* parent) {
    this->type = type;
    this->name = name;
    this->is_terminal = is_terminal;
    this->parent = parent;
    this->ID = NODE_COUNT++;
    this->line_no = yylineno;
    this->children = vector<node*>{};

    if (parent != nullptr) {
        parent->add_parent_child_relation(this);
    }
}

void node::add_parent_child_relation(node* child) {
    this->children.push_back(child);
    if(child != nullptr) {
        child->parent = this;
    }
}

void node::assign_value(string value) {
    this->str_val = value;
    this->val_repr = value;
}

void node::assign_value(long long int value) {
    this->int_val = value;
    this->val_repr = to_string(value);
}

void node::assign_value(double value) {
    this->double_val = value;
    this->val_repr = to_string(value);
}

void node::assign_value(bool value) {
    this->bool_val = value;
    this->val_repr = value ? "true" : "false";
}

void node::traverse_tree() {
    cout << "ID: " << this->ID << " | Type: " << this->type << " | Name: " << this->name << " | Line No: " << this->line_no << " | Value: " << this->name << endl;
    for (auto child : this->children) {
        if(child != nullptr) {
            child->traverse_tree();
        }
    }
}

void node::clean_tree() {
    // cout << "node.cpp::clean_tree() \ncurrently on node " << this->name << " and the children of this node are (size = " << this->children.size() << ")" << endl;
    // if a child is nullptr, simply delete the entry from the children vector
    auto newEnd = remove(this->children.begin(), this->children.end(), nullptr);
    this->children.erase(newEnd, children.end());
    
    // cout << "Will now call recursively" << endl;
    // recursively calling its children
    for (auto child : this->children) {
        if(child != nullptr) {
            // cout << "calling the function from child " << child->name << endl;
            child->clean_tree();
        }
    }
}

bool node::delete_delimiters() {
    int i;
    for(i = 0; i < this->children.size(); i++) {
        if(this->children[i] != nullptr) {
            if(this->children[i]->delete_delimiters() == true) {
                i--; // to ensure that all children are iterated over correctly
            }
        }
    }
    // if this node is a delimiter, then delete this node and free the memory
    if(find(delimiter_toks_vector.begin(), delimiter_toks_vector.end(), this->type) != delimiter_toks_vector.end()) {
        // cout << "Deleting delimiter: " << this->name << this->type << endl;
        // delete from the parent's children vector and free this node
        auto& siblings = this->parent->children;
        auto it = find(siblings.begin(), siblings.end(), this);
        if(it != siblings.end()) {  // this condition should never be false
            siblings.erase(it);
        }
        delete this; // this will not leak memory since this node is not a parent of any other node (it is a terminal node)
        return true;
    }
    else {
        return false;
    }
}

void node::delete_single_child_nodes() {
    for (auto child: this->children) {
        if(child != nullptr) {
            child->delete_single_child_nodes();
        }
    }
    // if only 1 child for this node, can delete this node and move the child to the parent
    if(this->children.size() == 1) { // terminals will have 0 children
        node* child = this->children.front();
        auto& siblings = this->parent->children;
        auto it = find(siblings.begin(), siblings.end(), this);
        if(it != siblings.end()) {  // this condition should never be false
            *it = child;
            child->parent = this->parent;
        }
        this->children.clear();
        delete this;
        return;
    }
}

void node::generate_dot_script() {
    //TODO: if the name of a node is a string then need to resolve below code, right now ignoring it
    dot_stream << "Node" << this->ID << "[label=\"" << this->name << "\"]\n";
    for(auto child : this->children) {
        if(child != nullptr) {
            child->generate_dot_script();
            dot_stream << "Node" << this->ID << "-> Node" << child->ID << endl;
        }
    }
    return;
}