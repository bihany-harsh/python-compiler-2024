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

extern vector<Quadruple*> IR;
extern long long int INTERMEDIATE_COUNTER;

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
    if (!this->is_terminal) {
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
            } else if ((it->first == INT_NUMBER) || (it->first == FLOAT_NUMBER) || (it->first == STRING_LITERAL)) {
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

void node::check_multi_assignment() {
    if(this->children.size() == 3) {
        return; // only a single assignment
    }
    // `this` is expr_stmt and it is before it is converted into an AST
    node* tmp;

    for(int i = 0; i < this->children.size() - 1; i++) {
        tmp = this->children[i];
        if (tmp->type == TEST) {
            // check_declare_before_use(SYMBOL_TABLE, this->children[i]);
            while(tmp && tmp->children.size() == 1) {
                tmp = tmp->children[0];
            }
            if (!tmp) {
                yyerror("UnexpectedError: tmp is nullptr.");
            }
            if (tmp->type != IDENTIFIER) {
                yyerror(("SyntaxError: " + tmp->name + " is not a valid lvalue.").c_str());
            }
        }
    }

    return;
}

void node::update_list_param() {
    // `this` is expr_stmt
    node* tmp;
    symbol_table_entry* list_entry;
    int children_size; 
    node* rval = this->children[this->children.size() - 1];
    while(rval && !rval->is_terminal && rval->type != ATOM) {
        rval = rval->children[0];
    }
    if (!rval) {
        yyerror("SyntaxError: not a valid assignment.");
    }
    if (rval->is_terminal) {
        return; // non-list assignment
    }
    // rval is now ATOM
    if (rval->children[0]->name != "[") {
        yyerror("SyntaxError: Improper list declaration.");
    }
    children_size = rval->children[1]->children.size();
    children_size = (children_size + (children_size % 2)) / 2; // because of commas

    for(int i = 0; i < this->children.size() - 1; i++) {
        tmp = this->children[i];
        if (tmp->type == TEST) {
            while(tmp && tmp->children.size() == 1) {
                tmp = tmp->children[0];
            }
            if (!tmp) {
                yyerror("UnexpectedError: tmp is nullptr.");
            }
            // if (tmp->type == ATOM_EXPR) {
            //     // array assignment not to be hindered!
            //     return; // already returned near the beginning of the function
            // }
            if (tmp->type != IDENTIFIER) {
                yyerror(("SyntaxError: " + tmp->name + " is not a valid lvalue.").c_str());
            }
            list_entry = SYMBOL_TABLE->get_entry(tmp->name); 
            if (list_entry->b_type == D_LIST) {
                if (list_entry->l_attr.num_of_elems != -1) {
                    yyerror("Re-initialization of lists not supported. Please wait for a newer version.");
                }
                // initialization
                list_entry->l_attr.num_of_elems = children_size;
            }
            else { // type checking: LHS is not a list but RHS is
                yyerror("TypeError: Not a valid list assignment.");
            }
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

void node::handle_annassign() {
    this->delete_single_child_nodes();
    
    if (this == nullptr || this->children[1]->type != ANNASSIGN) {
        cout << "SHOULD NOT HAPPEN in handle_annasign!!" << endl;
        return;
    }
    node* annassign = this->children[1];
    node* ID = this->children[0];
    ID->add_parent_child_relation(annassign->children[1]); // the first child is TOK_COLON (removed in post processing)
    annassign->children[1] = nullptr;
    if (annassign->children.size() != 2) { // delim and test
        // when initialization
        for(int i = 2; i < annassign->children.size(); i++) {
            this->add_parent_child_relation(annassign->children[i]);
            annassign->children[i] = nullptr;
        }
    }
    this->children[1] = nullptr;
    auto newEnd = remove(this->children.begin(), this->children.end(), nullptr);
    this->children.erase(newEnd, children.end()); 
    set<string> match = {"="};

    // delete annassign to avoid mem-leak
    delete annassign->children[0];
    delete annassign;

    to_ast_operator(this, false, match);
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
    if(tmp->type != IDENTIFIER && (tmp->name != "range" && tmp->name != "print" && tmp->name != "len")) {
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
                return D_LIST;
                // if(tmp->children[1]->children[1]->type == INT) {
                //     return D_LIST_INT;
                // } else if(tmp->children[1]->children[1]->type == FLOAT) {
                //     return D_LIST_FLOAT;
                // } else if(tmp->children[1]->children[1]->type == BOOL) {
                //     return D_LIST_BOOL;
                // } else if(tmp->children[1]->children[1]->type == STR) {
                //     return D_LIST_STRING;
                // } else if(tmp->children[1]->children[1]->type == IDENTIFIER) {
                //     //TODO: verify if correct (we can have a list of objects of user defined classes)
                //     st_entry* entry = st->get_entry(tmp->children[1]->children[1]->name);
                //     if(entry && entry->b_type == D_CLASS) {
                //         return D_LIST_CLASS;
                //     } else {
                //         yyerror("Not a valid rvalue");
                //     }
                // } else {
                //     yyerror("Not a valid rvalue");
                // }
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

void node::set_list_attributes(node* annassign) {
    node* tmp = annassign->children[1]; // points to ```test```
    while(tmp && tmp->children.size() != 2) {
        tmp = tmp->children[0];
    }
    
    if(!tmp || tmp->type != ATOM_EXPR) {
        yyerror("Unexpected error when setting list attributes.\nExiting");
    }
    // this->st_entry->l_attr.list_elem_type = tmp->children[1]->children[1]
    if (tmp->children[1]->children[0]->name != "[") {
        yyerror("SyntaxError: Improper list declaration.");
    }
    if(tmp->children[1]->children[1]->type == IDENTIFIER) {
        // trying to declare a list of classes
        symbol_table_entry* entry = st->get_entry(tmp->children[1]->children[1]->name);
        if(entry && entry->b_type == D_CLASS) {
            this->st_entry->l_attr.list_elem_type = D_CLASS;
            this->st_entry->l_attr.class_name = entry->name;
        }
        else {
            yyerror("TypeError: Not a valid type for a list");
        }
    }
    else {
        // list of basic data type
        switch(tmp->children[1]->children[1]->type) {
            case INT:
                this->st_entry->l_attr.list_elem_type = D_INT;
            break;
            case FLOAT:
                this->st_entry->l_attr.list_elem_type = D_FLOAT;
            break;
            case BOOL:
                this->st_entry->l_attr.list_elem_type = D_BOOL;
            break;
            case STRING_LITERAL:
                this->st_entry->l_attr.list_elem_type = D_STRING;
            break;
            default:
                yyerror("TypeError: Not a valid type for a list");
            break;
        }
    }
    if(annassign->children.size() > 2) {
        // list has an initialization
        tmp = annassign->children[3]; // this also points to ```test```
        while(tmp && tmp->children.size() == 1) {
            tmp = tmp->children[0];
        }
        if(!tmp || tmp->type != ATOM || tmp->children[0]->name != "[") {
            yyerror("TypeError: Not a valid rvalue");
        }
        int children_size = tmp->children[1]->children.size(); 
        this->st_entry->l_attr.num_of_elems = (children_size + (children_size % 2)) / 2;
    }
    else {
        this->st_entry->l_attr.num_of_elems = -1; // no initialization
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

// void return_type_check(node* test) {
//     base_data_type return_type = SYMBOL_TABLE->parent->get_entry(SYMBOL_TABLE->st_name)->f_attr.return_type;
//     // the current scope is that of a function and hence finding it in the parent
//     node* tmp = test;
// } 

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
    // optional_arglist->traverse_tree();
    if(optional_arglist && optional_arglist->children.size() > 4) {
        yyerror("CompilationError: multiple inheritances not supported.");
    }
    if (!optional_arglist || optional_arglist->children.size() == 2) {
        // no inheritance
        return;
    }
    check_declare_before_use(SYMBOL_TABLE->parent, optional_arglist);
    cout << "checking decl before use" << endl;
    node* tmp = optional_arglist->children[1]->children[0]; // optional_paren_arglist-->argument->test
    tmp->traverse_tree();
    while (tmp && !tmp->is_terminal) {
        if (tmp->children.size() > 1) {
            yyerror("SyntaxError: Invalid base class.");
        }
        tmp = tmp->children[0];
    }
    symbol_table_entry* tmp_entry = SYMBOL_TABLE->parent->get_entry(tmp->name);
    if (!tmp || tmp->type != IDENTIFIER || tmp_entry->b_type != D_CLASS) {
        yyerror("SyntaxError: Inheritance should be from proper base class.");
    }
    // should be able to inherit base class attributes now
    for(symbol_table_entry* inherit_entry: tmp_entry->child_symbol_table->entries) {
        if (inherit_entry->b_type == D_FUNCTION && inherit_entry->name != "__init__") {
            // check if the function is already defined in the current class
            symbol_table_entry* present_entry = SYMBOL_TABLE->get_entry(inherit_entry->name);
            if (!present_entry) {
                SYMBOL_TABLE->add_entry(inherit_entry);
            } else {
                if (present_entry->b_type == D_FUNCTION) {
                    if (inherit_entry->f_attr.num_args != present_entry->f_attr.num_args) {
                        SYMBOL_TABLE->add_entry(inherit_entry);
                    } else {
                        bool to_inherit = false;
                        for(int i = 0; i < inherit_entry->f_attr.num_args; i++) {
                            if (inherit_entry->f_attr.args[i] != present_entry->f_attr.args[i]) {
                                to_inherit = true;
                                break;
                            }
                        }
                        if (to_inherit) {
                            SYMBOL_TABLE->add_entry(inherit_entry);
                        }
                    }
                }
            }
        } else if (inherit_entry->b_type == D_CLASS) {
            // TODO: throw an incomapatibilty error whenever a member of a class is a some other class 
        } else {
            if (!SYMBOL_TABLE->get_entry(inherit_entry->name)) {
                SYMBOL_TABLE->add_entry(inherit_entry);
            }
            // else ignore, as the member is now overwritten for this class
        }
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

base_data_type max_operand_type(base_data_type lhs, base_data_type rhs) {
    if(lhs == D_FLOAT || rhs == D_FLOAT) {
        return D_FLOAT;
    }
    else if(lhs == D_INT || rhs == D_INT) {
        return D_INT;
    }
    else if(lhs == D_BOOL || rhs == D_BOOL) {
        return D_BOOL;
    }
    else if(lhs == D_STRING || rhs == D_STRING) {
        return D_STRING;
    }
    else {
        yyerror("UnexpectedTypeError: Invalid operand types.");
    }
    return D_VOID; //gcc shutup!
}

string node::get_lhs_operand() {
    if(this->children.size() != 2 && this->type != UNARY_OP) {
        yylineno = this->line_no;
        yyerror("UnexpectedError: node does not have 2 children");
    }
    switch(this->children[0]->type) {
        case IDENTIFIER:
            this->children[0]->operand_type = SYMBOL_TABLE->get_entry(this->children[0]->name)->b_type;
            return this->children[0]->name;
        case STRING_LITERAL:
            this->children[0]->operand_type = D_STRING;
            return this->children[0]->name;
        case BOOL_NUMBER:
            this->children[0]->operand_type = D_BOOL;
            return this->children[0]->name;
        case INT_NUMBER:
            this->children[0]->operand_type = D_INT;
            return this->children[0]->name;
        case FLOAT_NUMBER:
            this->children[0]->operand_type = D_FLOAT;
            return this->children[0]->name;
        case UNARY_OP:
            this->children[0]->operand_type = this->children[0]->children[0]->operand_type;
            return this->children[0]->_3acode->result;
        case BIN_OP:
            this->children[0]->operand_type = max_operand_type(this->children[0]->children[0]->operand_type, this->children[0]->children[1]->operand_type); // operand type would be same as that of the child
            return this->children[0]->_3acode->result;
        case ATOM_EXPR:
            if(this->children[0]->children[0]->name == "print" || this->children[0]->children[0]->name == "self" || this->children[0]->children[0]->name == "range" || this->children[0]->children[0]->name == "len") {
                yyerror("not handling this yet");
            }
            if(!SYMBOL_TABLE->get_entry(this->children[0]->children[0]->name)) {
                yyerror("UnexpectedError: Couldn't find entry of the list variable");
            }
            this->children[0]->operand_type = SYMBOL_TABLE->get_entry(this->children[0]->children[0]->name)->l_attr.list_elem_type;
            return this->children[0]->_3acode->result;
        case KEYWORD:
            if(this->name != "print" || this->name != "self" || this->name != "range" || this->name != "len") {
                yyerror("UnexpectedError: pyparse grammar is violated.");
            }
            
    }
    //TODO: clear up
    return this->children[0]->_3acode->result;
}

string node::get_rhs_operand() {
    if(this->children.size() != 2) {
        yylineno = this->line_no;
        yyerror("UnexpectedError: node does not have 2 children");
    }
    switch(this->children[1]->type) {
        case IDENTIFIER:
            this->children[1]->operand_type = SYMBOL_TABLE->get_entry(this->children[1]->name)->b_type;
            return this->children[1]->name;
        case FLOAT_NUMBER:
            this->children[1]->operand_type = D_FLOAT;
            return this->children[1]->name;
        case INT_NUMBER:
            this->children[1]->operand_type = D_INT;
            return this->children[1]->name;
        case STRING_LITERAL:
            this->children[1]->operand_type = D_STRING;
            return this->children[1]->name;
        case BOOL_NUMBER:
            this->children[1]->operand_type = D_BOOL;
            return this->children[1]->name;
        case ATOM_EXPR:
            if(this->children[1]->children[0]->name == "print" || this->children[1]->children[0]->name == "self" || this->children[1]->children[0]->name == "range" || this->children[1]->children[0]->name == "len") {
                yyerror("not handling this yet");
            }
            if(!SYMBOL_TABLE->get_entry(this->children[1]->children[0]->name)) {
                yyerror("UnexpectedError: Couldn't find entry of the list variable");
            }
            this->children[1]->operand_type = SYMBOL_TABLE->get_entry(this->children[1]->children[0]->name)->l_attr.list_elem_type;
            return this->children[1]->_3acode->result;
        case UNARY_OP:
            this->children[1]->operand_type = this->children[1]->children[0]->operand_type;
            return this->children[1]->_3acode->result;
        case BIN_OP:
            this->children[1]->operand_type = max_operand_type(this->children[1]->children[0]->operand_type, this->children[1]->children[1]->operand_type);
            // this->children[1]->operand_type = this->children[1]->children[1]->operand_type; // operand type would be same as that of the child
            return this->children[1]->_3acode->result;
        case ASSIGN:
            this->children[1]->operand_type = this->children[1]->children[0]->operand_type;
            return this->children[1]->_3acode->result;
    }
    //TODO: clear this up
    return "";
}

void node::check_operand_type_compatibility() {
    string temp_result; // stores the temporary result attr of a Quadruple (for coercion)
    Quadruple* q; // temporary, for adding coercion statements to the Intermediate code
    switch(this->_3acode->q_type) {
        case Q_UNARY:
            if(this->children[0]->operand_type == D_STRING) {
                yylineno = this->children[0]->line_no;
                yyerror("TypeError: Incompatible operation with string.");
            }
            else if(this->children[0]->operand_type == D_FLOAT) {
                if(this->name == "~") {
                    yylineno = this->children[0]->line_no;
                    yyerror("TypeError: unsupported operand type for ~: 'float'");
                }
            }
            else if(this->children[0]->operand_type == D_BOOL) {
                temp_result = "t" + to_string(INTERMEDIATE_COUNTER++);
                if(this->children[0]->_3acode != nullptr) {
                    // there is some temporary result stored, that has to be coerced
                    q = new Quadruple("=", "int", this->children[0]->_3acode->result, temp_result, Q_COERCION);
                } else {
                    q = new Quadruple("=", "int", this->children[0]->name, temp_result, Q_COERCION);
                }
                this->children[0]->operand_type = D_INT;
                IR.push_back(q);
                this->_3acode->rename_attribute(ARG1, temp_result);
            }
        break;
        case Q_INDEX:
            if(this->children[1]->operand_type == D_BOOL) {
                temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                if(this->children[1]->_3acode != nullptr) {
                    // there is some temporary result stored, that has to be coerced
                    q = new Quadruple("=", "int", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                } else {
                    q = new Quadruple("=", "int", this->children[1]->name, temp_result, Q_COERCION);
                }
                this->children[1]->operand_type = D_INT;
                IR.push_back(q);
                this->_3acode->rename_attribute(ARG2, temp_result); //FIXME: check that the correct attr is renamed
            }
            else if(this->children[1]->operand_type != D_INT) {
                yylineno = this->children[0]->line_no;
                yyerror("IndexError: list index must be an integer");
            }
        break;
        case Q_BINARY:
            switch(this->children[0]->operand_type) {
                case D_BOOL:
                    if(this->children[1]->operand_type == D_INT) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                        if(this->children[0]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "int", this->children[0]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "int", this->children[0]->name, temp_result, Q_COERCION);
                        }
                        this->children[0]->operand_type = D_INT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG1, temp_result);
                    }
                    else if(this->children[1]->operand_type == D_FLOAT) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                        if(this->children[0]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "float", this->children[0]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "float", this->children[0]->name, temp_result, Q_COERCION);
                        }
                        this->children[0]->operand_type = D_FLOAT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG1, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_BOOL) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatyes");
                    }
                break;
                case D_INT:
                    if(this->children[1]->operand_type == D_BOOL) {
                        cout << "This is the correct place" << endl;
                        temp_result = "t" + to_string(INTERMEDIATE_COUNTER++);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "int", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "int", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_INT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG2, temp_result);
                    }
                    else if(this->children[1]->operand_type == D_FLOAT) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                        // q = new Quadruple("=", "float", this->children[0]->name, temp_result, Q_COERCION);
                        if(this->children[0]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "float", this->children[0]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "float", this->children[0]->name, temp_result, Q_COERCION);
                        }
                        this->children[0]->operand_type = D_FLOAT;
                        IR.push_back(q);
                        // this->_3acode->arg1 = temp_result;
                        this->_3acode->rename_attribute(ARG1, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_INT) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatypes");
                    }
                break;
                case D_FLOAT:
                    if(this->children[1]->operand_type == D_BOOL) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                        // q = new Quadruple("=", "float", this->children[1]->name, temp_result, Q_COERCION);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "float", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "float", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_FLOAT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG2, temp_result);
                    }
                    else if(this->children[1]->operand_type == D_INT) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                        // q = new Quadruple("=", "float", this->children[1]->name, temp_result, Q_COERCION);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "float", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "float", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_FLOAT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG2, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_FLOAT) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatypes");
                    }
                break;
                case D_STRING:
                    if(this->name == "+" || this->name == "+=") {
                        yylineno = this->children[1]->line_no;
                        yyerror("String concatenation not supported yet. Please wait for a newer version.");
                    }
                    else {
                        yylineno = this->children[1]->line_no;
                        yyerror("TypeError: Incompatible operation with string");
                    }
                break;
                default:
                //TODO: Assigning value to an array index?
                    yylineno = this->children[0]->line_no;
                    yyerror("TypeError: Incompatible datatypes");
                break;
            }
        break;
        case Q_ASSIGN:
            // cout << "---------------" << endl;
            // cout << this->name << endl;
            // cout << this->children[0]->operand_type << " " << this->children[1]->operand_type << endl;
            switch(this->children[0]->operand_type) {
                case D_BOOL:
                    if(this->children[1]->operand_type == D_STRING) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Cannot assign a string to a boolean");
                    }
                    else if(this->children[1]->operand_type == D_INT || this->children[1]->operand_type == D_FLOAT) {
                        temp_result = "t" + to_string(INTERMEDIATE_COUNTER++);
                        // q = new Quadruple("=", "bool", this->children[1]->name, temp_result, Q_COERCION);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "bool", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "bool", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_BOOL;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG2, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_BOOL) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatypes");
                    }
                break;
                case D_INT:
                    if(this->children[1]->operand_type == D_BOOL || this->children[1]->operand_type == D_FLOAT) {
                        temp_result = "t" + to_string(INTERMEDIATE_COUNTER++);
                        // q = new Quadruple("=", "int", this->children[1]->name, temp_result, Q_COERCION);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "int", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "int", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_INT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG2, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_INT) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatypes");
                    }
                break;
                case D_FLOAT:
                    if(this->children[1]->operand_type == D_BOOL || this->children[1]->operand_type == D_INT) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++)).c_str();
                        // q = new Quadruple("=", "float", this->children[1]->name, temp_result, Q_COERCION);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "float", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "float", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_FLOAT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG2, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_FLOAT) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatypes");
                    }
                break;
                case D_STRING:
                    if(this->children[1]->operand_type != D_STRING) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Cannot assign string to a non-string.");
                    }
                break;
            }
    }
    return;
}

void node::generate_3ac() {
    for(auto child: this->children) {
        if(child != nullptr) {
            // cout << "before calling " << child->name << ", IC = " << INTERMEDIATE_COUNTER << endl;
            if(this->type == FILE_INPUT) {
                INTERMEDIATE_COUNTER = 1; // resetting it for each statement
            }
            child->generate_3ac();
        }
    }
    string op1, op2, result, op;
    switch(this->type) {
        case KEYWORD:
        case INT:
        case FLOAT:
        case BOOL:
        case STR:
        case LIST:
        case IDENTIFIER:
        case STRING_LITERAL:
        case INT_NUMBER:
        case BOOL_NUMBER:
        case FLOAT_NUMBER:
            return;
        case ATOM_EXPR:
            if(this->children[0]->name == "print") {
                op1 = this->get_rhs_operand();
                this->_3acode = new Quadruple("", op1, "", "", Q_PRINT);
                // TODO: any type checking here?
                IR.push_back(this->_3acode);
            }
            else if(this->children[0]->name == "self" || this->children[0]->name == "range" || this->children[0]->name == "len") {
                yyerror("Not supporting this yet");
            }
            else if(this->children[0]->type == IDENTIFIER) {
                op1 = this->get_lhs_operand();
                op2 = this->get_rhs_operand(); //TODO: multiply by width. 
                result = "t" + to_string(INTERMEDIATE_COUNTER++);
                this->_3acode = new Quadruple("", op1, op2, result, Q_INDEX);
                this->check_operand_type_compatibility();
                IR.push_back(this->_3acode);
                //TODO: add instructions for dereferencing
            }
            return;
        case UNARY_OP:
            op1 = this->get_lhs_operand();
            result = "t" + to_string(INTERMEDIATE_COUNTER++);
            this->_3acode = new Quadruple(this->name, op1, "", result, Q_UNARY);
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        case ASSIGN:
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            this->_3acode = new Quadruple(this->name, op2, "", op1, Q_ASSIGN);
            this->check_operand_type_compatibility();
            // cout << "After assign, IC = " << INTERMEDIATE_COUNTER << endl;
            IR.push_back(this->_3acode);
            return;
        case BIN_OP:
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            result = "t" + to_string(INTERMEDIATE_COUNTER++);
            this->_3acode = new Quadruple(this->name, op1, op2, result, Q_BINARY);
            this->check_operand_type_compatibility();
            // cout << "After bin, IC = " << INTERMEDIATE_COUNTER << endl;
            IR.push_back(this->_3acode);
            return;
        case AUGASSIGN:
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            result = op1;
            this->_3acode = new Quadruple(this->name.substr(0, this->name.length() - 1), op1, op2, result, Q_BINARY); // removing "=" from the name
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        default:
            // yyerror("Unexpected error: generate_3ac called on a non-terminal node");
            return;
    }
}