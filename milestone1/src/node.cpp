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
    cout << "ID: " << this->ID << " | Type: " << this->type << " | Name: " << this->name << " | Line No: " << this->line_no << " | Value: " << this->name << " | Is_terminal " << this->is_terminal << endl;
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
    if(find(delimiter_vector.begin(), delimiter_vector.end(), this->type) != delimiter_vector.end()) {
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
    for (auto& child : this->children) {
        if(child != nullptr) {
            child->delete_single_child_nodes();
        }
    }

    if(this->children.size() == 1) { // terminals will have 0 children
        node* child = this->children.front();

        // Check if this node is not the root node (it has a parent)
        if(this->parent != nullptr) {
            // Find this node among its siblings and replace it with its child
            auto& siblings = this->parent->children;
            auto it = std::find(siblings.begin(), siblings.end(), this);
            if(it != siblings.end()) {
                *it = child;
                child->parent = this->parent;
            }

            // Clear children list to prevent deleting the child when this node is deleted
            this->children.clear();

            // Delete this node
            delete this;
        }
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

int get_operator_precedence(node_type type) {
    auto it = operator_precedence.find(type);
    if (it != operator_precedence.end()) {
        return it->second;
    }
    return numeric_limits<int>::max();
}

bool is_right_associative(node_type type) {
    return type == DOUBLE_STAR ||
           type == EQUAL ||
           type == PLUS_EQUAL ||
           type == MINUS_EQUAL ||
           type == STAR_EQUAL ||
           type == SLASH_EQUAL ||
           type == DOUBLE_SLASH_EQUAL ||
           type == PERCENT_EQUAL ||
           type == DOUBLE_STAR_EQUAL ||
           type == AMPER_EQUAL ||
           type == VBAR_EQUAL ||
           type == CIRCUMFLEX_EQUAL ||
           type == LEFT_SHIFT_EQUAL ||
           type == RIGHT_SHIFT_EQUAL;
}

void prune_custom_nodes(node* parent, node* child) {
    if (child) {
        for (auto child_: child->children) {
            parent->add_parent_child_relation(child_);
        }
        delete child;
    }
}

// void ast_conv_operators(node *root) {
//     if (root == nullptr) return;

//     vector<node*> children_copy = root->children;
//     for (auto child: children_copy) {
//         ast_conv_operators(child);
//     }

//     if ((operator_set.find(root->type) != operator_set.end()) && root->parent != nullptr && (operator_set.find(root->parent->type) == operator_set.end())) {
//         node* parent = root->parent;
//         if(parent->parent != nullptr) {
//             vector<node*>& siblings = parent->parent->children;
//             auto it = find(siblings.begin(), siblings.end(), parent);
//             if (it != siblings.end()) {
//                 *it = root;
//             }
//             root->parent = parent->parent;
//         }
//         for(auto* sibling: parent->children) {
//             if(sibling != root) {
//                 if (operator_set.find(sibling->type) == operator_set.end()) {
//                     root->add_parent_child_relation(sibling);
//                 } else {
//                     // complete this part of the function mainting the precedence and associativity
//                 }
//             } 
//         }
//         parent->children.clear();
//         delete parent;
//     }
// }

void ast_conv_operators(node *root) {
    if (root == nullptr) return;

    // Perform depth-first traversal to process all children first
    std::vector<node*> children_copy = root->children;
    for (auto* child : children_copy) {
        ast_conv_operators(child);
    }

    // Check if the current node is an operator
    if (operator_set.find(root->type) != operator_set.end()) {
        // Check if the parent exists and is not an operator of higher precedence
        while (root->parent != nullptr &&
               operator_set.find(root->parent->type) == operator_set.end()) {
            node* parent = root->parent;
            
            // Check if the parent is an operator with lower or same precedence
            if (operator_set.find(parent->type) != operator_set.end() &&
                (get_operator_precedence(parent->type) > get_operator_precedence(root->type) ||
                (get_operator_precedence(parent->type) == get_operator_precedence(root->type) &&
                 !is_right_associative(root->type)))) {
                // Break the loop if the parent operator has higher precedence or is right associative
                break;
            }

            // Re-parenting: make the operator node a direct child of the grandparent node
            if (parent->parent != nullptr) {
                auto& siblings = parent->parent->children;
                auto it = std::find(siblings.begin(), siblings.end(), parent);
                if (it != siblings.end()) {
                    *it = root;
                }
                root->parent = parent->parent;
            } else {
                root->parent = nullptr; // Current node becomes the root of the tree
            }

            // Transfer the siblings that are not operators or have lower precedence
            // to be children of the current operator node
            for (auto* sibling : parent->children) {
                if (sibling != root &&
                    (operator_set.find(sibling->type) == operator_set.end() ||
                     get_operator_precedence(sibling->type) > get_operator_precedence(root->type))) {
                    root->children.push_back(sibling);
                    sibling->parent = root;
                }
            }

            // Clear the parent's children to avoid double deletion
            parent->children.clear();

            // Delete the parent node as it has been replaced in the AST
            delete parent;
        }
    }
}
