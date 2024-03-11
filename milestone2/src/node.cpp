#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>
#include "include/node.hpp"
using namespace std;

long long int NODE_COUNT = 0;

node* AST_ROOT = NULL;

extern int yylineno;
extern ostringstream dot_stream;
extern void yyerror(const char*);

extern symbol_table* SYMBOL_TABLE;
extern stack<symbol_table*> ST_STACK;

extern stack<int> OFFSET_STACK;
extern int OFFSET;
extern int block_counter;

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
        // if(child != nullptr) { // this condiiton is not required since already removed the nullptr children
        child->clean_tree();
        // }
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
    if(this->type == DELIMITER && (this->name != "[" || this->name != "]")) {
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

    // terminals have 0 children
    if(this->children.size() == 1 && unary_ops.find(this->type) == unary_ops.end()) { // it is not a unary operator
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
    if(this->is_terminal) {
        auto it = type_map.find(this->type);
        if(it != type_map.end()) {
            if (operator_set.find(it->first) != operator_set.end())
                dot_stream << "Node" << this->ID << "[label=" << "\"" << it->second << "(" << this->name << ")\"" << "fillcolor=indianred1 style=filled" << "]\n";
            else if (it->first == IDENTIFIER) {
                dot_stream << "Node" << this->ID << "[label=" << "\"" << it->second << "(" << this->name << ")\"" << "fillcolor=lightskyblue style=filled" << "]\n";
            } else if ((it->first == NUMBER) || (it->first == STRING_LITERAL)) {
                dot_stream << "Node" << this->ID << "[label=" << "\"" << it->second << "(" << this->name << ")\"" << "fillcolor=aquamarine style=filled" << "]\n";
            } else if (it->first == KEYWORD) {
                dot_stream << "Node" << this->ID << "[label=" << "\"" << it->second << "(" << this->name << ")\"" << "fillcolor=plum style=filled" << "]\n";
            } else {
                dot_stream << "Node" << this->ID << "[label=" << "\"" << it->second << "(" << this->name << ")\"" << "fillcolor=lightgoldenrodyellow style=filled" << "]\n";
            }
        }
        else {
            cout << "node.cpp: line 145: unexpected error" << endl;
        }
    }
    else {
        dot_stream << "Node" << this->ID << "[label=\"" << this->name << "\"" << "fillcolor=aliceblue style=filled shape=hexagon" << "]\n";
    }
    for(auto child : this->children) {
        if(child != nullptr) {
            child->generate_dot_script();
            dot_stream << "Node" << this->ID << "-> Node" << child->ID << endl;
        }
    }
    return;
}

void prune_custom_nodes(node* parent, node* child) {
    if (child) {
        for (auto child_: child->children) {
            if(child_) { // child_ is non-null
                parent->add_parent_child_relation(child_);
            }
        }
        delete child;
    }
}

void to_ast_operator(node* root, bool is_left_associative, set<string> matching_strings) {
    if(is_left_associative) {
        for(int i = 0; i < root->children.size(); i++) {
            node* child = root->children[i];
            if(matching_strings.find(child->name) != matching_strings.end()) {
                child->add_parent_child_relation(root->children[i - 1]);
                child->add_parent_child_relation(root->children[i + 1]);
                // now to remove earlier relations
                root->children[i - 1] = NULL;
                root->children[i + 1] = NULL;
                // now to remove the NULL children
                auto newEnd = remove(root->children.begin(), root->children.end(), nullptr);
                root->children.erase(newEnd, root->children.end());
                i--;
            }
        }
    }
    else {
        for(int i = root->children.size() - 1; i >= 0; i--) {
            if(root->children.size() == 1) break;
            node* child = root->children[i];
            if(matching_strings.find(child->name) != matching_strings.end()) {
                child->add_parent_child_relation(root->children[i - 1]);
                child->add_parent_child_relation(root->children[i + 1]);
                // now to remove earlier relations
                root->children[i - 1] = NULL;
                root->children[i + 1] = NULL;
                // now to remove the NULL children
                auto newEnd = remove(root->children.begin(), root->children.end(), nullptr);
                root->children.erase(newEnd, root->children.end());
                i--;
            }
        }
    }
}

void comp_op_processing(node* root) {
    int modified = 0;
    for(int i = 0; i < root->children.size(); i++) {
        node* child = root->children[i];
        if(child->type == COMPARE) {
            // we have a comparison operator
            modified = 1;
            child->add_parent_child_relation(root->children[i - 1]);
            child->add_parent_child_relation(root->children[i + 1]);

            // now remove the earlier relations
            root->children[i - 1] = NULL;
            // root->children[i + 1] = NULL;
            auto newEnd = remove(root->children.begin(), root->children.end(), nullptr);
            root->children.erase(newEnd, root->children.end());
            i--;
        }
    }
    if(modified) {
        if(!root->children.empty()) {
            root->children.pop_back();
        }
        while(root->children.size() > 1) {
            node* new_node = new node(BOOL_OP, "and", true, NULL);
            new_node->add_parent_child_relation(root->children.front());
            root->children.erase(root->children.begin());
            new_node->add_parent_child_relation(root->children.front());
            root->children.erase(root->children.begin());
            // we cannot directly use add_parent_child_relation because we have to maintain relative order
            new_node->parent = root;
            root->children.insert(root->children.begin(), new_node); // insert at the beginning
        }
    }
}

void handle_annassign(node* root) {
    if(root == nullptr) {
        return;
    }

    if(root->type == ANNASSIGN) {
        // if there is an ANNASSIGN node, its parent will DEFINITELY be expr_stmt
        if(root->children.size() == 1) {
            // if there is only 1 child, it has to be the specified type. Then, we simply add it in the place of annassign.
            node* parent = root->parent;
            for(int i = 0; i < parent->children.size(); i++) {
                if(parent->children[i] == root) {
                    parent->children[i] = root->children[0];    // establishing parent
                    root->children[0]->parent = parent;         // child relationship
                    delete root;
                    return;
                }
            }
        }
    }

    for(int i = 0; i < root->children.size(); i++) {
        handle_annassign(root->children[i]);
    }
}

node* sem_lval_check(node* root) {
    node* tmp = root;
    while(tmp && (!tmp->is_terminal)) {
        if(tmp->children.size() > 1) {
            if(SYMBOL_TABLE->st_type == CLASS || (SYMBOL_TABLE->st_type == FUNCTION && SYMBOL_TABLE->parent->st_type == CLASS && SYMBOL_TABLE->st_name == "__init__")) {
                return nullptr;
            }
            yyerror("Not a valid lvalue");
        }
        tmp = tmp->children[0];
    }
    if(!tmp) {
        yyerror("Not a valid lvalue");
    }
    if(tmp->type != IDENTIFIER && (tmp->name != "range" && tmp->name != "print")) {
        yyerror((tmp->name + " is not a valid lvalue").c_str());
    }
    return tmp;
}

base_data_type sem_rval_check(symbol_table* st, node* root) {
    //  root points to the node of ```test``` in the grammar
    node* tmp = root;
    while(tmp && (!tmp->is_terminal)) {
        if(tmp->children.size() > 1) {
            if(tmp->type != ATOM_EXPR) {
                yyerror("Not a valid rvalue");
            }
            else {
                if(tmp->children[1]->children[1]->type == INT) {
                    return D_LIST_INT;
                } else if(tmp->children[1]->children[1]->type == FLOAT) {
                    return D_LIST_FLOAT;
                } else if(tmp->children[1]->children[1]->type == BOOL) {
                    return D_LIST_BOOL;
                } else if(tmp->children[1]->children[1]->type == STR) {
                    return D_LIST_STRING;
                } else if(tmp->children[1]->children[1]->type == IDENTIFIER) {
                    //TODO: verify if correct (we can have a list of objects of user defined classes)
                    st_entry* entry = st->get_entry(tmp->children[1]->children[1]->name);
                    if(entry && entry->b_type == D_CLASS) {
                        return D_LIST_CLASS;
                    } else {
                        yyerror("Not a valid rvalue");
                    }
                } else {
                    yyerror("Not a valid rvalue");
                }
            }
        }
        tmp = tmp->children[0];
    }
    if(!tmp) {
        yyerror("Not a valid rvalue");
    }
    if(tmp->type == INT) {
        return D_INT;
    } else if(tmp->type == FLOAT) {
        return D_FLOAT;
    } else if(tmp->type == BOOL) {
        return D_BOOL;
    } else if(tmp->type == STR) {
        return D_STRING;
    } else if(tmp->type == IDENTIFIER) {
        st_entry* entry = st->get_entry(tmp->name);
        if(entry && entry->b_type == D_CLASS) {
            return entry->b_type;
        } else {
            yyerror((tmp->name + " is not defined").c_str());
        }
    } else if(tmp->type == KEYWORD && tmp->name == "None") {
        return D_VOID;
    } else {
        yyerror("Not a valid rvalue");
    }
    return D_VOID; // gcc shutup!
}

void check_declare_before_use(symbol_table* st, node* root) {
    if(root == nullptr)
        return;
    if(root->type == IDENTIFIER) {
        if(!st->get_entry(root->name)) {
            yyerror((root->name + " is not defined").c_str());
        }
    }
    for(int i = 0; i < root->children.size(); i++) {
        check_declare_before_use(st, root->children[i]);
    }
}

void node::setup_new_st_env() {
    this->st = SYMBOL_TABLE; // this entry belongs to the parent symbol table
    ST_STACK.push(SYMBOL_TABLE);
    SYMBOL_TABLE = new symbol_table(BLOCK, ("IF_BLOCK" + to_string(++block_counter)).c_str(), ST_STACK.top());
    OFFSET_STACK.push(OFFSET);
    OFFSET = 0;
    return;
}

void node::create_block_st(const char* block_name) {
    if(OFFSET > 0) {
        this->st_entry = new symbol_table_entry((block_name + to_string(block_counter)).c_str(), D_BLOCK, OFFSET_STACK.top(), this->line_no, SYMBOL_TABLE->parent->scope);
        this->st_entry->set_size(OFFSET);
        this->st_entry->child_symbol_table = SYMBOL_TABLE;
        SYMBOL_TABLE = ST_STACK.top();
        OFFSET = OFFSET_STACK.top();
        SYMBOL_TABLE->add_entry(this->st_entry);
        OFFSET += this->st_entry->size;
    }
    else {
        delete SYMBOL_TABLE; // preventing a memory leak
        SYMBOL_TABLE = ST_STACK.top();
        OFFSET = OFFSET_STACK.top();
    }
    ST_STACK.pop();
    OFFSET_STACK.pop();
    return;
}

void node::create_func_st() {
    this->st = SYMBOL_TABLE;
    // for functions, we create the st_entry before executing the body so that we can add arguments
    this->st_entry = new symbol_table_entry(this->name, D_FUNCTION, OFFSET, this->line_no, SYMBOL_TABLE->scope);
    // create a symbol table for the function scope
        // first store the current symbol table and offset
    ST_STACK.push(SYMBOL_TABLE);
    OFFSET_STACK.push(OFFSET);
        // create a new symbol table
    SYMBOL_TABLE = new symbol_table(FUNCTION, this->name, ST_STACK.top());
    OFFSET = 0;
    return;
}

void node::exit_from_func() {
    cout << "exit from function called for function " << this->name << endl;
    // if TFPDEF has a single child, it can only be self and the current environment must be a class environment
    // in all other cases, TFPDEF has > 1 children. The type is specified by the descendent of TFPDEP->children[2] and the name is specified by TFPDEF->children[0]
    this->st_entry->set_size(OFFSET);
    this->st_entry->child_symbol_table = SYMBOL_TABLE;
    //TODO: handle that non-default arguments appear before default args
    bool found_default_arg = false; // to check that typed (non-default arguments) are before typed (default) arguments
    // non-default arguments are arguments that do not have optional_equal_test
    cout << "num_args = " << this->st_entry->f_attr.num_args << endl;
    for(int i = 0; i < this->st_entry->f_attr.num_args; i++) {
        symbol_table_entry* arg = SYMBOL_TABLE->entries[i];
        // cout << "obtained the argument entry" << endl;
        this->st_entry->f_attr.args.push_back(arg->b_type);
    }

    SYMBOL_TABLE = ST_STACK.top();
    OFFSET = OFFSET_STACK.top();
    SYMBOL_TABLE->add_entry(this->st_entry);
    OFFSET += this->st_entry->size;
    ST_STACK.pop();
    OFFSET_STACK.pop();
    cout << "Completed the function exit_from_func" << endl;
    return;
}

void node::create_class_st() {
    this->st = SYMBOL_TABLE;
    this->st_entry = new symbol_table_entry(this->name, D_CLASS, OFFSET, this->line_no, SYMBOL_TABLE->scope);
    // SYMBOL_TABLE->add_entry(this->st_entry);
    ST_STACK.push(SYMBOL_TABLE);
    OFFSET_STACK.push(OFFSET);
    SYMBOL_TABLE = new symbol_table(CLASS, this->name, ST_STACK.top());
    OFFSET = 0;
}

void add_class_st_entry(node* test, base_data_type b_type) {
    // checks are done previosuly before the call to this function (like "__init__")
    node* tmp = test;
    while (tmp && tmp->children.size() == 1) {
        tmp = tmp->children[0];
    }
    if (!tmp) {
        yyerror("Unexpected error.");
    }
    if (tmp->type != ATOM_EXPR || tmp->children.size() != 2) {
        yyerror("Not a valid lvalue.");
    }
    if (tmp->children[0]->name != "self") {
        yyerror((tmp->children[0]->name + " is not a valid lvalue.").c_str());
    }
    if (tmp->children[1]->children[0]->name != ".") {
        yyerror("SyntaxError: invalid usage of self.");
    }
    st_entry* entry = new st_entry(tmp->children[1]->children[1]->name, b_type, OFFSET, tmp->children[1]->children[1]->line_no, SYMBOL_TABLE->scope);
    OFFSET += entry->size;
    if (SYMBOL_TABLE->st_type == CLASS) {
        SYMBOL_TABLE->add_entry(entry);
    } else if (SYMBOL_TABLE->st_type == FUNCTION) {
        SYMBOL_TABLE->parent->add_entry(entry);
    } else {
        yyerror("Unexpected error");
    }
}

void node::handle_inheritance(node* optional_arglist) {
    if(optional_arglist && optional_arglist->children.size() > 4) {
        yyerror("CompilationError: multiple inheritances not supported.");
    }
    if (!optional_arglist || optional_arglist->children.size() == 2) {
        // no inheritance
        return;
    }
    check_declare_before_use(SYMBOL_TABLE->parent, optional_arglist);
    node* tmp = optional_arglist->children[0]->children[0]; // optional_arglist-->arglist-->argument
    while(tmp && !tmp->is_terminal) {
        if (tmp->children.size() > 1) {
            yyerror("SyntaxError: Invalid base class.");
        }
        tmp = tmp->children[0];
    }
    if (!tmp || tmp->type != IDENTIFIER || tmp->st_entry->b_type != D_CLASS) {
        yyerror("SyntaxError: Inheritance should be from proper base class.");
    }
    // should be able to inherit base class attributes now
    for(symbol_table_entry* inherit_entry: tmp->st->entries) {
        // TODO: check for functions and inherit approprately.
    }
}

void node::exit_from_class() {
    this->st_entry->set_size(OFFSET);
    this->st_entry->child_symbol_table = SYMBOL_TABLE;
    SYMBOL_TABLE = ST_STACK.top();
    OFFSET = OFFSET_STACK.top();
    SYMBOL_TABLE->add_entry(this->st_entry);
    OFFSET += this->st_entry->size;
    ST_STACK.pop();
    OFFSET_STACK.pop();
    return;
}
