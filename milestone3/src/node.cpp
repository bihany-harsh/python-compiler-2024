#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stack>
#include <utility>
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
extern stack<int> LABEL_CNT_STACK;
extern int LABEL_COUNTER;
extern bool pending_return, pending_init;

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
            if(SYMBOL_TABLE->st_type == FUNCTION && SYMBOL_TABLE->parent->st_type == CLASS) {
                return nullptr;
            } else if (SYMBOL_TABLE->st_type == CLASS) {
                yyerror("IncompatiblityError: Stray code in class not supported.");
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
            // return entry->b_type;
            return D_CLASS;
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

string get_class_name(node* test) {
    // assuming we directly get the class name
    node* tmp = test;  
    while(tmp && (!tmp->is_terminal)) {
        tmp = tmp->children[0];
    }
    return tmp->name;
}

void set_list_elem_type(symbol_table* st, node* test, st_entry* new_entry) {
    //  root points to the node of ```test``` in the grammar
    node* tmp = test;
    while(tmp && (!tmp->is_terminal)) {
        if(tmp->children.size() > 1) {
            if(tmp->type != ATOM_EXPR) {
                yyerror("Not a valid rvalue");
            }
            else {
                if (tmp->children[1]->children[1]->name == "int") {
                    new_entry->l_attr.list_elem_type = D_INT;
                } else if (tmp->children[1]->children[1]->name == "float") {
                    new_entry->l_attr.list_elem_type = D_FLOAT;
                } else if (tmp->children[1]->children[1]->name == "bool") {
                    new_entry->l_attr.list_elem_type = D_BOOL;
                } else if (tmp->children[1]->children[1]->name == "string") {
                    new_entry->l_attr.list_elem_type = D_STRING;
                } else if (tmp->children[1]->children[1]->type == IDENTIFIER) {
                    // list of clases
                    if ((!SYMBOL_TABLE->get_entry(tmp->children[1]->children[1]->name)) || (SYMBOL_TABLE->get_entry(tmp->children[1]->children[1]->name)->b_type != D_CLASS)) {
                        yyerror("TypeError: Not a valid list declaration.");    
                    }
                    new_entry->l_attr.list_elem_type = D_CLASS;
                    new_entry->l_attr.class_name = tmp->children[1]->name;
                } else {
                    yyerror("TypeError: Not a valid list declaration.");
                }
            }
        }
        tmp = tmp->children[0];
    }
}

void check_declare_before_use(symbol_table* st, node* root) {
    // TODO: self.a should be declared before use
    if(root == nullptr)
        return;
    if(root->type == IDENTIFIER) {
        if(!st->get_entry(root->name) && root->parent->children[0]->name != ".") {
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
            this->st_entry->l_attr.list_elem_size = entry->size;
            this->st_entry->class_name = entry->name;
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
                this->st_entry->l_attr.list_elem_size = 4;
            break;
            case FLOAT:
                this->st_entry->l_attr.list_elem_type = D_FLOAT;
                this->st_entry->l_attr.list_elem_size = 8;
            break;
            case BOOL:
                this->st_entry->l_attr.list_elem_type = D_BOOL;
                this->st_entry->l_attr.list_elem_size = 1;
            break;
            case STRING_LITERAL:
                this->st_entry->l_attr.list_elem_type = D_STRING;
                this->st_entry->l_attr.list_elem_size = 8;
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
    // if TFPDEF has a single child, it can only be self and the current environment must be a class environment
    // in all other cases, TFPDEF has > 1 children. The type is specified by the descendent of TFPDEP->children[2] and the name is specified by TFPDEF->children[0]
    //TODO: handle that non-default arguments appear before default args
    bool found_default_arg = false; // to check that typed (non-default arguments) are before typed (default) arguments
    // non-default arguments are arguments that do not have optional_equal_test
    for(int i = 0; i < this->st_entry->f_attr.num_args; i++) {
        symbol_table_entry* arg = SYMBOL_TABLE->entries[i];
        // cout << "obtained the argument entry" << endl;
        this->st_entry->f_attr.args.push_back(arg->b_type);
        if (arg->b_type == D_LIST) {
            // cout << arg->l_attr.list_elem_type << endl;
            if(arg->l_attr.list_elem_type == D_BOOL) {
                this->st_entry->f_attr.list_types.push_back("bool");
            } else if(arg->l_attr.list_elem_type == D_INT) {
                this->st_entry->f_attr.list_types.push_back("int");
            } else if(arg->l_attr.list_elem_type == D_FLOAT) {
                this->st_entry->f_attr.list_types.push_back("float");
            } else if(arg->l_attr.list_elem_type == D_STRING) {
                this->st_entry->f_attr.list_types.push_back("string");
            } else if(arg->l_attr.list_elem_type == D_CLASS) {
                this->st_entry->f_attr.list_types.push_back(SYMBOL_TABLE->get_entry(arg->l_attr.class_name)->name);
            } else {
                yyerror("UnexpectedError: List is not of a valid type");
            }
        } else {
            this->st_entry->f_attr.list_types.push_back("void");
        }
    }
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
    st_entry* entry = new st_entry("self." + tmp->children[1]->children[1]->name, b_type, OFFSET, tmp->children[1]->children[1]->line_no, SYMBOL_TABLE->scope);
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
    node* tmp = optional_arglist->children[1]->children[0]; // optional_paren_arglist-->argument->test
    while (tmp && !tmp->is_terminal) {
        if (tmp->children.size() > 1) {
            yyerror("SyntaxError: Invalid base class.");
        }
        tmp = tmp->children[0];
    }
    symbol_table_entry* new_entry;
    symbol_table_entry* tmp_entry = SYMBOL_TABLE->parent->get_entry(tmp->name); // entry of the class to be inherited
    if (!tmp || tmp->type != IDENTIFIER || tmp_entry->b_type != D_CLASS) {
        yyerror("SyntaxError: Inheritance should be from proper base class.");
    }
    // should be able to inherit base class attributes now
    for(symbol_table_entry* inherit_entry: tmp_entry->child_symbol_table->entries) {
        if (inherit_entry->b_type == D_FUNCTION && inherit_entry->name != "__init__") {
            // check if the function is already defined in the current class
            symbol_table_entry* present_entry = SYMBOL_TABLE->get_entry(inherit_entry->name);
            if (!present_entry) {
                new_entry = new symbol_table_entry(inherit_entry->name, inherit_entry->b_type, OFFSET, inherit_entry->decl_line, SYMBOL_TABLE->scope);
                OFFSET += new_entry->size;
                copy_func_attr(inherit_entry, new_entry);
                SYMBOL_TABLE->add_entry(new_entry);
            } else {
                if (present_entry->b_type == D_FUNCTION) {
                    if (inherit_entry->f_attr.num_args != present_entry->f_attr.num_args) {
                        new_entry = new symbol_table_entry(inherit_entry->name, inherit_entry->b_type, OFFSET, inherit_entry->decl_line, SYMBOL_TABLE->scope);
                        OFFSET += new_entry->size;
                        copy_func_attr(inherit_entry, new_entry); // 
                        SYMBOL_TABLE->add_entry(new_entry);
                    } else {
                        bool to_inherit = false;
                        for(int i = 0; i < inherit_entry->f_attr.num_args; i++) {
                            if (inherit_entry->f_attr.args[i] != present_entry->f_attr.args[i]) {
                                to_inherit = true;
                                break;
                            } else if(inherit_entry->f_attr.list_types[i] != present_entry->f_attr.list_types[i]) {
                                to_inherit = true;
                                break;
                            }
                        }
                        if (to_inherit) {
                            new_entry = new symbol_table_entry(inherit_entry->name, inherit_entry->b_type, OFFSET, inherit_entry->decl_line, SYMBOL_TABLE->scope);
                            OFFSET += new_entry->size;
                            copy_func_attr(inherit_entry, new_entry);
                            SYMBOL_TABLE->add_entry(new_entry);
                        }
                    }
                }
            }
        } else if (inherit_entry->b_type == D_CLASS) {
            // TODO: throw an incomapatibilty error whenever a member of a class is a some other class 
        } else {
            if (!SYMBOL_TABLE->get_entry(inherit_entry->name)) {
                new_entry = new symbol_table_entry(inherit_entry->name, inherit_entry->b_type, OFFSET, inherit_entry->decl_line, SYMBOL_TABLE->scope);
                OFFSET += new_entry->size;
                SYMBOL_TABLE->add_entry(new_entry);
            }
            // else ignore, as the member is now overwritten for this class
        }
    }
}

void node::exit_from_class() {
    this->st_entry->child_symbol_table = SYMBOL_TABLE;
    this->st_entry->set_size(0);
    SYMBOL_TABLE = ST_STACK.top();
    OFFSET = OFFSET_STACK.top();
    SYMBOL_TABLE->add_entry(this->st_entry);
    OFFSET += this->st_entry->size;
    ST_STACK.pop();
    OFFSET_STACK.pop();

    this->st_entry->child_symbol_table->sort_class_entries();
    return;
}

void node::add_break_node(node* break_node) {
    this->break_nodes.push_back(break_node);
    return;
}

void node::add_continue_node(node* continue_node) {
    this->continue_nodes.push_back(continue_node);
    return;
}

node* find_loop_ancestor(node* root) {
    if(root == nullptr || root->type == FILE_INPUT) {
        return nullptr;
    }
    if(root->type == FOR_STMT || root->type == WHILE_STMT) {
        return root->children[0];
    }
    return find_loop_ancestor(root->parent);
}

string make_function_label(node* funcdef) {
    string label = "";
    node* parameters = funcdef->children[2];
    if (SYMBOL_TABLE->parent->st_type == CLASS) {
        label = SYMBOL_TABLE->parent->st_name + "_" + funcdef->children[1]->name;; // name of the class + name of the function
        if(parameters->children[0]->name != "self") {
            yyerror("IncompatibilityError: Class member methods must have a `self` argument.");
        }
        for (int i = 1; i < parameters->children.size(); i++) { // from 1 because 0 is self
            label += "_" + parameters->children[i]->children[1]->name;
        }
    } else {
        label = funcdef->children[1]->name;
        for(node* tfpdef: parameters->children) {
            label += "_" + tfpdef->children[1]->name;
        }
    }
    return label;
}

pair<bool,bool> check_coerce_required(pair<base_data_type, string> formal, pair<base_data_type, string> actual) {
    // first bool will denote compatibility and second bool will denote requirement of coercion
    switch(formal.first) {
        case D_BOOL:
            if (actual.first == D_BOOL) {
                return {true, false};
            }
            else if (actual.first == D_INT || actual.first == D_FLOAT) {
                return {true, true};
            }
            else {
                return {false, true};
            }
        break;
        case D_INT:
            if (actual.first == D_INT) {
                return {true, false};
            }
            else if (actual.first == D_FLOAT || actual.first == D_BOOL) {
                return {true, true};
            }
            else {
                return {false, true};
            }
        break;
        case D_FLOAT:
            if (actual.first == D_FLOAT) {
                return {true, false};
            }
            else if (actual.first == D_INT || actual.first == D_BOOL) {
                return {true, true};
            }
            else {
                return {false, true};
            }
        break;
        case D_STRING:
            if (actual.first == D_STRING) {
                return {true, false};
            }
            else {
                return {false, true};
            }
        break;
        case D_LIST:
            if (actual.first == D_LIST) {
                if(formal.second == actual.second) {
                    return {true, false};
                }
                else {
                    return {false, true};
                }
            }
            else {
                return {false, true};
            }
        break;
        case D_CLASS:
            if (actual.first == D_CLASS) {
                return {true, false};
            }
            else {
                return {false, true};
            }
        break;
        case D_VOID:
            if (actual.first == D_VOID) {
                return {true, false};
            }
            else {
                return {false, true};
            }
        break;
        default:
            return {false, true};
        break;
    }
}

symbol_table_entry* call_class_init(node* atom_expr, symbol_table* class_st) {
    st_entry* init_func;
    node* class_name = atom_expr->children[0];
    node* trailer = atom_expr->children[1];
    pair<bool,bool> compatibility_result;
    string data_type, result;
    Quadruple* q;
    for(st_entry* entry: class_st->entries) {
        if(entry->name == "__init__") {
            if(entry->b_type != D_FUNCTION) {
                yyerror(("SematicError: __init__ cannot be a data member for class" + class_name->name).c_str());
            }
            init_func = entry;
            break;
        }
    }
    if(trailer->children.size() != init_func->f_attr.num_args) {
        yylineno = atom_expr->line_no;
        yyerror("SyntaxError: Could not find a suitable init function for the given arguments");
    }
    // we now have the init function. We need to check the compatibility of arguments
    for(int i = 0; i < init_func->f_attr.num_args; i++) {
        if(trailer->children[i]->operand_type == D_LIST) {
            if(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type == D_CLASS) {
                compatibility_result = check_coerce_required({init_func->f_attr.args[i], init_func->f_attr.list_types[i]}, {trailer->children[i]->operand_type, SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.class_name});
            }
            else {
                compatibility_result = check_coerce_required({init_func->f_attr.args[i], init_func->f_attr.list_types[i]}, {trailer->children[i]->operand_type, base_data_type_map.find(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type)->second});
            }
        }
        else {
            compatibility_result = check_coerce_required({init_func->f_attr.args[i], init_func->f_attr.list_types[i]}, {trailer->children[i]->operand_type, "void"});
        }
        if(compatibility_result.first == false) {
            yylineno = atom_expr->line_no;
            yyerror("SyntaxError: Arguments cannot be coerced to the given definition of __init__ function.");
        }
    }
    // arguments are compatible, pushing them on the stack now

    for(int i = init_func->f_attr.num_args - 1; i >= 0; i--) {
        if(trailer->children[i]->operand_type == D_LIST) {
            if(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type == D_CLASS) {
                compatibility_result = check_coerce_required({init_func->f_attr.args[i], init_func->f_attr.list_types[i]}, {trailer->children[i]->operand_type, SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.class_name});
            }
            else {
                compatibility_result = check_coerce_required({init_func->f_attr.args[i], init_func->f_attr.list_types[i]}, {trailer->children[i]->operand_type, base_data_type_map.find(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type)->second});
            }
        }
        else {
            compatibility_result = check_coerce_required({init_func->f_attr.args[i], init_func->f_attr.list_types[i]}, {trailer->children[i]->operand_type, "void"});
        }

        if(init_func->f_attr.args[i] == D_BOOL) {
            data_type = "bool";
        }
        else if (init_func->f_attr.args[i] == D_INT) {
            data_type = "int";
        }
        else if (init_func->f_attr.args[i] == D_FLOAT) {
            data_type = "float";
        } else if (init_func->f_attr.args[i] == D_STRING) {
            data_type = "string";
        } else if (init_func->f_attr.args[i] == D_LIST) {
            data_type = "list";
            data_type += init_func->f_attr.list_types[i];
        } else if (init_func->f_attr.args[i] == D_CLASS) {
            data_type = "class";
        } else if (init_func->f_attr.args[i] == D_VOID) {
            data_type = "void";
        }
        else {
            yyerror("UnexpectedError: cannot be coerced");
        }

        if (compatibility_result.second == true) { // coercion required
            if(trailer->children[i]->_3acode != nullptr) {
                q = new Quadruple("=", data_type, trailer->children[i]->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                result = q->result;
                IR.push_back(q);
            }
            else {
                q = new Quadruple("=", data_type, trailer->children[i]->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                result = q->result;
                IR.push_back(q);
            }
        } else {
            if(trailer->children[i]->_3acode != nullptr) {
                result = trailer->children[i]->_3acode->result;
            }
            else {
                result = trailer->children[i]->name;
            }
        }
        q = new Quadruple("", "", "", result, Q_PUSH_PARAM);
        IR.push_back(q);
    }
    if(atom_expr->parent->type != ASSIGN) {
        yylineno = atom_expr->line_no;
        yyerror("SyntaxError: Class initiailization cannot be a part of the expression, it is the expression.");
    }
    q = new Quadruple("", "", "", atom_expr->parent->children[0]->name, Q_PUSH_PARAM);
    IR.push_back(q);
    return init_func;
}

symbol_table_entry* call_class_member_method(node* atom_expr, symbol_table* class_st) {
    bool found = false;
    bool incompatible_args = false;
    node* func_name = atom_expr->children[1]->children[1];
    node* trailer = atom_expr->children[2];
    Quadruple* q;
    string data_type, result;
    st_entry* func_entry;
    pair<bool, bool> compatibility_result;
    symbol_table* st = SYMBOL_TABLE;

    for(st_entry* st_entry: class_st->entries) {
            if ((st_entry->b_type == D_FUNCTION) && (st_entry->name == func_name->name)) {
            incompatible_args = false;
            if (st_entry->f_attr.num_args == trailer->children.size()) {
                for(int i = 0; i < trailer->children.size(); i++) {
                    if(trailer->children[i]->operand_type == D_LIST) {
                        if(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type == D_CLASS) {
                            compatibility_result = check_coerce_required({st_entry->f_attr.args[i], st_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.class_name});
                        }
                        else {
                            compatibility_result = check_coerce_required({st_entry->f_attr.args[i], st_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, base_data_type_map.find(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type)->second});
                        }
                    }
                    else {
                        compatibility_result = check_coerce_required({st_entry->f_attr.args[i], st_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, "void"});
                    }
                    if(compatibility_result.first == false) {
                        incompatible_args = true;
                        break;
                    }
                }
            }
            else {
                incompatible_args = true;
            }
            if (incompatible_args == false) {
                if (found) {
                    yylineno = atom_expr->line_no;
                    yyerror(("Call of function " + func_name->name + " is ambiguous").c_str());
                }
                else {
                    found = true;
                    func_entry = st_entry;
                }
            }
        }
    }
    if(!found) {
        yylineno = trailer->line_no;
        yyerror("TypeError: Could not find a function matching this definition.");
    }

    atom_expr->operand_type = func_entry->f_attr.return_type;

    for(int i = func_entry->f_attr.num_args - 1; i >= 0; i--) {
        if(trailer->children[i]->operand_type == D_LIST) {
            if(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type == D_CLASS) {
                compatibility_result = check_coerce_required({func_entry->f_attr.args[i], func_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.class_name});
            }
            else {
                compatibility_result = check_coerce_required({func_entry->f_attr.args[i], func_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, base_data_type_map.find(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type)->second});
            }
        }
        else {
            compatibility_result = check_coerce_required({func_entry->f_attr.args[i], func_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, "void"});
        }

        if(func_entry->f_attr.args[i] == D_BOOL) {
            data_type = "bool";
        }
        else if (func_entry->f_attr.args[i] == D_INT) {
            data_type = "int";
        }
        else if (func_entry->f_attr.args[i] == D_FLOAT) {
            data_type = "float";
        } else if (func_entry->f_attr.args[i] == D_STRING) {
            data_type = "string";
        } else if (func_entry->f_attr.args[i] == D_LIST) {
            data_type = "list";
            data_type += func_entry->f_attr.list_types[i];
        } else if (func_entry->f_attr.args[i] == D_CLASS) {
            data_type = "class";
        } else if (func_entry->f_attr.args[i] == D_VOID) {
            data_type = "void";
        }
        else {
            yyerror("UnexpectedError: cannot be coerced");
        }

        if (compatibility_result.second == true) { // coercion required

            if(trailer->children[i]->_3acode != nullptr) {
                q = new Quadruple("=", data_type, trailer->children[i]->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                result = q->result;
                IR.push_back(q);
            }
            else {
                q = new Quadruple("=", data_type, trailer->children[i]->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                result = q->result;
                IR.push_back(q);
            }
        } else {
            if(trailer->children[i]->_3acode != nullptr) {
                result = trailer->children[i]->_3acode->result;
            }
            else {
                result = trailer->children[i]->name;
            }
        }
        q = new Quadruple("", "", "", result, Q_PUSH_PARAM);
        IR.push_back(q);
    }
    q = new Quadruple("", "", "", atom_expr->children[0]->name, Q_PUSH_PARAM);
    IR.push_back(q);
    return func_entry;
}

symbol_table_entry* get_compatible_function_and_push_param(node* atom_expr) {
    bool found = false;
    bool incompatible_args = false;
    node* func_name = atom_expr->children[0];
    node* trailer = atom_expr->children[1];
    Quadruple* q;
    string data_type, result;
    st_entry* func_entry;
    pair<bool, bool> compatibility_result;
    symbol_table* st = SYMBOL_TABLE;
    while(st) {
        for (st_entry* st_entry: st->entries) {
            incompatible_args = false;
            if ((st_entry->b_type == D_FUNCTION) && (st_entry->name == func_name->name)) {
                if (st_entry->f_attr.num_args == trailer->children.size()) {
                    for(int i = 0; i < trailer->children.size(); i++) {
                        if(trailer->children[i]->operand_type == D_LIST) {
                            if(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type == D_CLASS) {
                                compatibility_result = check_coerce_required({st_entry->f_attr.args[i], st_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.class_name});
                            }
                            else {
                                compatibility_result = check_coerce_required({st_entry->f_attr.args[i], st_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, base_data_type_map.find(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type)->second});
                            }
                        }
                        else {
                            compatibility_result = check_coerce_required({st_entry->f_attr.args[i], st_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, "void"});
                        }
                        if(compatibility_result.first == false) {
                            incompatible_args = true;
                            break;
                        }
                    }
                }
                else {
                    incompatible_args = true;
                }
                if (incompatible_args == false) {
                    if (found) {
                        yylineno = atom_expr->line_no;
                        yyerror(("Call of function " + func_name->name + " is ambiguous").c_str());
                    }
                    else {
                        found = true;
                        func_entry = st_entry;
                    }
                }
            }
        }
        st = st->parent;
        if (found) 
            break;
    }

    if(!found) {
        yylineno = trailer->line_no;
        yyerror("TypeError: Could not find a function matching this definition.");
    }
    for(int i = func_entry->f_attr.num_args - 1; i >= 0; i--) {
        if(trailer->children[i]->operand_type == D_LIST) {
            if(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type == D_CLASS) {
                compatibility_result = check_coerce_required({func_entry->f_attr.args[i], func_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.class_name});
            }
            else {
                compatibility_result = check_coerce_required({func_entry->f_attr.args[i], func_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, base_data_type_map.find(SYMBOL_TABLE->get_entry(trailer->children[i]->name)->l_attr.list_elem_type)->second});
            }
        }
        else {
            compatibility_result = check_coerce_required({func_entry->f_attr.args[i], func_entry->f_attr.list_types[i]}, {trailer->children[i]->operand_type, "void"});
        }

        if(func_entry->f_attr.args[i] == D_BOOL) {
            data_type = "bool";
        }
        else if (func_entry->f_attr.args[i] == D_INT) {
            data_type = "int";
        }
        else if (func_entry->f_attr.args[i] == D_FLOAT) {
            data_type = "float";
        } else if (func_entry->f_attr.args[i] == D_STRING) {
            data_type = "string";
        } else if (func_entry->f_attr.args[i] == D_LIST) {
            data_type = "list";
            data_type += func_entry->f_attr.list_types[i];
        } else if (func_entry->f_attr.args[i] == D_CLASS) {
            data_type = "class";
        } else if (func_entry->f_attr.args[i] == D_VOID) {
            data_type = "void";
        }
        else {
            yyerror("UnexpectedError: cannot be coerced");
        }

        if (compatibility_result.second == true) { // coercion required

            if(trailer->children[i]->_3acode != nullptr) {
                q = new Quadruple("=", data_type, trailer->children[i]->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                result = q->result;
                IR.push_back(q);
            }
            else {
                q = new Quadruple("=", data_type, trailer->children[i]->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                result = q->result;
                IR.push_back(q);
            }
        } else {
            if(trailer->children[i]->_3acode != nullptr) {
                result = trailer->children[i]->_3acode->result;
            }
            else {
                result = trailer->children[i]->name;
            }
        }
        q = new Quadruple("", "", "", result, Q_PUSH_PARAM);
        IR.push_back(q);
    }

    // return f_label;
    return func_entry;
}

void check_type_and_gen_3ac_return_stmt(node* funcdef, node* return_stmt) {
    // if(!pending_return) {
    //     yylineno = return_stmt->line_no;
    //     yyerror("SyntaxError: Multiple return statements inside a single function");
    // }
    pending_return = false; // encountered a return statement in a function, so this flag is again marked false

    base_data_type return_type_formal = funcdef->children[1]->st_entry->f_attr.return_type; // funcdef->children[1] is the IDENTIFIER node
    int num_args = funcdef->children[1]->st_entry->f_attr.num_args;
    base_data_type return_type_actual;
    Quadruple* q = nullptr;
    if (return_stmt->children.size() != 1) {
        switch (return_stmt->children[1]->type) {
            case INT_NUMBER:
            case FLOAT_NUMBER:
            case BOOL_NUMBER:
            case STRING_LITERAL:
                return_type_actual = return_stmt->children[1]->operand_type;
                break;
            case IDENTIFIER:
                return_type_actual = SYMBOL_TABLE->get_entry(return_stmt->children[1]->name)->b_type;
                break;
            default:
                if (return_stmt->children[1]->_3acode != nullptr) {
                    return_type_actual = return_stmt->children[1]->operand_type;
                } else {
                    yylineno = return_stmt->line_no;
                    yyerror("UnexpectedError: Invalid return type.");
                }
        }
    } else {
        // no second child, only `return` keyword, returning void
        return_type_actual = D_VOID;
    }

    switch (return_type_formal) {
        case D_BOOL:
            if (return_type_actual == D_INT || return_type_actual == D_FLOAT) {
                if (return_stmt->children[1]->_3acode != nullptr) {
                    q = new Quadruple("=", "bool", return_stmt->children[1]->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                } else {
                    q = new Quadruple("=", "bool", return_stmt->children[1]->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                }
                IR.push_back(q);
                funcdef->_3acode = new Quadruple("return", "", "", q->result, Q_JUMP);
                IR.push_back(funcdef->_3acode);
            }
            else if (return_type_actual == D_BOOL) {
                if (return_stmt->children[1]->_3acode != nullptr) {
                    funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->_3acode->result, Q_JUMP);
                }
                else {
                    funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->name, Q_JUMP);
                }
                IR.push_back(funcdef->_3acode);
            }
            else {
                yylineno = return_stmt->line_no;
                yyerror("TypeError: argument does not match return type.");
            }
            break;
        case D_INT:
            if (return_type_actual == D_FLOAT || return_type_actual == D_BOOL) {
                if (return_stmt->children[1]->_3acode != nullptr) {
                    q = new Quadruple("=", "int", return_stmt->children[1]->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                } else {
                    q = new Quadruple("=", "int", return_stmt->children[1]->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                }
                IR.push_back(q);
                funcdef->_3acode = new Quadruple("return", "", "", q->result, Q_JUMP);
                IR.push_back(funcdef->_3acode);
            }
            else if (return_type_actual == D_INT) {
                if (return_stmt->children[1]->_3acode != nullptr) {
                    funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->_3acode->result, Q_JUMP);
                }
                else {
                    funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->name, Q_JUMP);
                }
                IR.push_back(funcdef->_3acode);
            }
            else {
                yylineno = return_stmt->line_no;
                yyerror("TypeError: argument does not match return type.");
            }
            break;
        case D_FLOAT:
            if (return_type_actual == D_INT || return_type_actual == D_BOOL) {
                if (return_stmt->children[1]->_3acode != nullptr) {
                    q = new Quadruple("=", "float", return_stmt->children[1]->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                } else {
                    q = new Quadruple("=", "float", return_stmt->children[1]->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                }
                IR.push_back(q);
                funcdef->_3acode = new Quadruple("return", "", "", q->result, Q_JUMP);
                IR.push_back(funcdef->_3acode);
            }
            else if (return_type_actual == D_FLOAT) {
                if (return_stmt->children[1]->_3acode != nullptr) {
                    funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->_3acode->result, Q_JUMP);
                }
                else {
                    funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->name, Q_JUMP);
                }
                IR.push_back(funcdef->_3acode);
            }
            else {
                yylineno = return_stmt->line_no;
                yyerror("TypeError: argument does not match return type.");
            }
            break;
        case D_STRING:
            if (return_type_actual != D_STRING) {
                yylineno = return_stmt->line_no;
                yyerror("TypeError: argument does not match return type.");
            }
            funcdef->_3acode = new Quadruple("return", "", "", return_stmt->children[1]->name, Q_JUMP);
            break;
        case D_VOID:
            if (return_type_actual != D_VOID) {
                yylineno = return_stmt->line_no;
                yyerror("TypeError: argument does not match return type.");
            }
            break;
        // TODO: check for D_CLASS and D_LIST
        default:
            yyerror("UnexpectedError: Invalid return type.");
            break;
    }
}

void do_list_assignment(node* assign) {
    // should be called from the node ASSIGN(=)
    string op1, temp_result, temp_value, size;
    st_entry* entry;
    Quadruple* q;

    op1 = assign->get_lhs_operand();
    entry = SYMBOL_TABLE->get_entry(op1);
    for(int i = 0; i < assign->children[1]->children.size(); i++) {
        node* temp = assign->children[1]->children[i];
        //TODO: list of class objs: not checked that the class objs be of the same type
        q = new Quadruple("*", to_string(i), to_string(entry->l_attr.list_elem_size), "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
        IR.push_back(q);
        q = new Quadruple("+", "addr(" + op1 + ")", q->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
        IR.push_back(q);
        temp_result = "*" + q->result;
        if(temp->_3acode == nullptr) {
            temp_value = temp->name;
        }
        else {
            temp_value = temp->_3acode->result;
        }
        switch(entry->l_attr.list_elem_type) { // the data type of the list elements
            case D_BOOL:
                if(temp->operand_type == D_STRING) {
                    yylineno = assign->children[0]->line_no;
                    yyerror("TypeError: Cannot assign a string to a boolean");
                }
                else if(temp->operand_type == D_INT || temp->operand_type == D_FLOAT) {
                    if(temp->_3acode != nullptr) {
                        // there is some temporary result stored, that has to be coerced
                        q = new Quadruple("=", "bool", temp->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                    } else {
                        q = new Quadruple("=", "bool", temp->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                    }
                    temp_value = q->result;
                    temp->operand_type = D_BOOL;
                    IR.push_back(q);
                }
                else if(temp->operand_type != D_BOOL) {
                    yylineno = assign->children[0]->line_no;
                    yyerror("TypeError: Incompatible datatypes");
                }
            break;
            case D_INT:
                if(temp->type == BOOL_NUMBER || temp->operand_type == D_FLOAT) {
                    // q = new Quadruple("=", "int", assign->children[1]->name, temp_result, Q_COERCION);
                    if(temp->_3acode != nullptr) {
                        // there is some temporary result stored, that has to be coerced
                        q = new Quadruple("=", "int", temp->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                    } else {
                        q = new Quadruple("=", "int", temp->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                    }
                    temp_value = q->result;
                    temp->operand_type = D_INT;
                    IR.push_back(q);
                }
                else if(temp->operand_type != D_INT) {
                    yylineno = assign->children[0]->line_no;
                    yyerror("TypeError: Incompatible datatypes");
                }
            break;
            case D_FLOAT:
                if(temp->operand_type == D_BOOL || temp->operand_type == D_INT) {
                    if(temp->_3acode != nullptr) {
                        // there is some temporary result stored, that has to be coerced
                        q = new Quadruple("=", "float", temp->_3acode->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                    } else {
                        q = new Quadruple("=", "float", temp->name, "t" + to_string(INTERMEDIATE_COUNTER++), Q_COERCION);
                    }
                    temp_value = q->result;
                    temp->operand_type = D_FLOAT;
                    IR.push_back(q);
                }
                else if(temp->operand_type != D_FLOAT) {
                    yylineno = assign->children[0]->line_no;
                    yyerror("TypeError: Incompatible datatypes");
                }
            break;
            case D_STRING:
                if(temp->operand_type != D_STRING) {
                    yylineno = assign->children[0]->line_no;
                    yyerror("TypeError: Cannot assign string to a non-string.");
                }
            break;
        }
        if(temp->_3acode == nullptr) {
            q = new Quadruple("=", temp_value, "", temp_result, Q_ASSIGN);
        }
        else {
            q = new Quadruple("=", temp_value, "", temp_result, Q_ASSIGN);
        }
        IR.push_back(q);
    }
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
    Quadruple* q;
    symbol_table_entry* entry;
    if(this->children.size() != 2 && this->type != UNARY_OP && this->type != TRAILER) { // range may get called with a single child
        yylineno = this->line_no;
        yyerror(("UnexpectedError: node " + this->name + " does not have 2 children").c_str());
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
        case COMPARE:
        case BOOL_OP:
            this->children[0]->operand_type = D_BOOL;
            return this->children[0]->_3acode->result;
        case ATOM_EXPR:
            if(this->children[0]->children[0]->name == "print") {
                yylineno = this->children[0]->children[0]->line_no;
                yyerror("SyntaxError: print cannot be part of an expression.");
            }
            else if(this->children[0]->children[0]->name == "self") {
                if(this->type != ASSIGN) { // if not the LHS of an assign
                    q = new Quadruple("", this->children[0]->_3acode->result, "", "t" + to_string(INTERMEDIATE_COUNTER++), Q_DEREFERENCE);
                    IR.push_back(q);
                    return q->result;
                }
                else { // if the left side of an assign
                    this->children[0]->_3acode->result = "*" + this->children[0]->_3acode->result;
                    return this->children[0]->_3acode->result;
                }
            }
            else if (this->children[0]->children[0]->name == "range" || this->children[0]->children[0]->name == "len") {
                yyerror("not handling this yet");
            }
            entry = SYMBOL_TABLE->get_entry(this->children[0]->children[0]->name);
            if(entry == nullptr) {
                yyerror("UnexpectedError: Couldn't find entry of the list variable");
            }
            if(entry->b_type == D_LIST) {
                this->children[0]->operand_type = SYMBOL_TABLE->get_entry(this->children[0]->children[0]->name)->l_attr.list_elem_type;
                // return this->children[0]->_3acode->result;
                if(this->type != ASSIGN) { // if not the LHS of an assign
                    q = new Quadruple("", this->children[0]->_3acode->result, "", "t" + to_string(INTERMEDIATE_COUNTER++), Q_DEREFERENCE);
                    IR.push_back(q);
                    return q->result;
                }
                else { // if the left side of an assign
                    this->children[0]->_3acode->result = "*" + this->children[0]->_3acode->result;
                    return this->children[0]->_3acode->result;
                }
            }
            else if(entry->b_type == D_FUNCTION) {
                this->children[0]->operand_type = entry->f_attr.return_type;
                return this->children[0]->_3acode->result; // FIXME: check this
            }
        case KEYWORD:
            if(this->name != "print" || this->name != "self" || this->name != "range" || this->name != "len") {
                yyerror("UnexpectedError: pyparse grammar is violated.");
            }
        case TRAILER:
            if (this->children[0]->children[0]->_3acode != nullptr) {
                return this->children[0]->children[0]->_3acode->result;
            } else {
                return this->children[0]->children[0]->name;
            }
            
    }
    //TODO: clear up
    return this->children[0]->_3acode->result;
}

string node::get_rhs_operand() {
    Quadruple* q;
    symbol_table_entry* entry;
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
            if(this->children[1]->children[0]->name == "print") {
                yylineno = this->children[0]->children[0]->line_no;
                yyerror("SyntaxError: print cannot be part of an expression.");
            }
            else if(this->children[1]->children[0]->name == "self") {
                return this->children[1]->_3acode->result;
            }
            else if(this->children[1]->children[0]->name == "range" || this->children[1]->children[0]->name == "len") {
                yyerror("not handling this yet");
            }
            entry = SYMBOL_TABLE->get_entry(this->children[1]->children[0]->name);
            if(entry == nullptr) {
                yyerror("UnexpectedError: Couldn't find entry of the list variable");
            }
            if(entry->b_type == D_LIST) {
                this->children[1]->operand_type = entry->l_attr.list_elem_type;
                q = new Quadruple("", this->children[1]->_3acode->result, "", "t" + to_string(INTERMEDIATE_COUNTER++), Q_DEREFERENCE);
                IR.push_back(q);
                return q->result;
            }
            else if(entry->b_type == D_FUNCTION) {
                this->children[1]->operand_type = entry->f_attr.return_type;
                return this->children[1]->_3acode->result; //FIXME: check this
            }
            else if(entry->b_type == D_CLASS) {
                // have called a member method of a class. operand_type is already set. simply return the result
                return this->children[1]->_3acode->result;
            }
        case UNARY_OP:
            this->children[1]->operand_type = this->children[1]->children[0]->operand_type;
            return this->children[1]->_3acode->result;
        case BIN_OP:
            this->children[1]->operand_type = max_operand_type(this->children[1]->children[0]->operand_type, this->children[1]->children[1]->operand_type);
            // this->children[1]->operand_type = this->children[1]->children[1]->operand_type; // operand type would be same as that of the child
            return this->children[1]->_3acode->result;
        case COMPARE:
        case BOOL_OP:
            this->children[1]->operand_type = D_BOOL;
            return this->children[1]->_3acode->result;
        case ASSIGN:
            this->children[1]->operand_type = this->children[1]->children[0]->operand_type;
            return this->children[1]->_3acode->result;
        case TRAILER:
            if (this->children[1]->children[0]->_3acode != nullptr) {
                return this->children[1]->children[0]->_3acode->result;
            } else {
                return this->children[1]->children[0]->name;
            }
    }
    //TODO: clear this up
    return "";
}

void node::check_operand_type_compatibility() {
    //TODO: check all the calls of the function rename_attribute() and ensure that correct attribute is renamed
    //TODO: ensure that all coercion cases are linked with a typecasting case
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
                        // cout << "This is the correct place" << endl;
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
                        yyerror("VersionError: String concatenation not supported yet. Please wait for a newer version.");
                    }
                    else if(this->type != COMPARE) {
                        yylineno = this->children[1]->line_no;
                        yyerror("TypeError: Incompatible operation with string");
                    }
                break;
                default:
                    yylineno = this->children[0]->line_no;
                    yyerror("TypeError: Incompatible datatypes");
                break;
            }
        break;
        case Q_ASSIGN:
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
                        this->_3acode->rename_attribute(ARG1, temp_result);
                    }
                    else if(this->children[1]->operand_type != D_BOOL) {
                        yylineno = this->children[0]->line_no;
                        yyerror("TypeError: Incompatible datatypes");
                    }
                break;
                case D_INT:
                    // cout << "this->children[1]->operand_type = " << this->children[1]->operand_type << endl;
                    if(this->children[1]->operand_type == D_BOOL || this->children[1]->operand_type == D_FLOAT) {
                        temp_result = "t" + to_string(INTERMEDIATE_COUNTER++);
                        if(this->children[1]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "int", this->children[1]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "int", this->children[1]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->operand_type = D_INT;
                        IR.push_back(q);
                        this->_3acode->rename_attribute(ARG1, temp_result);
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
                        this->_3acode->rename_attribute(ARG1, temp_result);
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


void node::generate_3ac_keywords() {
    // cout << "in generate_3ac_keyword " << this->name << endl;
    // catering to IF-ELIF-ELSE block
    string op1, op2, result;
    Quadruple* q;
    
    if ((this->name == "if") || (this->name == "elif")) { // backpatching for if-elif-else block
        this->children[0]->_3acode->rename_attribute(RESULT, to_string(LABEL_COUNTER));
        return;
    } else if ((this->name == "while") || (this->name == "for")) { // backpatching for while and for loops
        this->children[0]->_3acode->rename_attribute(RESULT, to_string(LABEL_COUNTER));
        for(node* break_node: this->break_nodes) {
            break_node->_3acode->rename_attribute(RESULT, to_string(LABEL_COUNTER));
        }

        this->children[1]->_3acode->rename_attribute(RESULT, to_string(LABEL_CNT_STACK.top()));
        for(node* continue_node: this->continue_nodes) {
            continue_node->_3acode->rename_attribute(RESULT, to_string(LABEL_CNT_STACK.top()));
        }
        LABEL_CNT_STACK.pop();
        return;
    } else if ((this->name == "break") || this->name == "continue") {
        // first need to ensure that this statement occured inside a loop
        node* tmp = find_loop_ancestor(this);
        if (tmp == nullptr) {
            yylineno = this->line_no;
            yyerror(("SyntaxError: '" + this->name + "' outside loop.").c_str());
        }
        if(this->name == "break") {
            tmp->break_nodes.push_back(this);
        }
        else {
            tmp->continue_nodes.push_back(this);
        }
        this->_3acode = new Quadruple("goto", "", "", "", Q_JUMP); // the label will be backpatched when we reach the end of the loop
        IR.push_back(this->_3acode);
    }
    else if ((this->name == "def")) {
        // function label creation
        this->_3acode = new Quadruple("", "begin_func", this->parent->children[1]->st_entry->label, "", Q_FUNC_LABEL);
        // this->_3acode->rename_attribute(ARG1, this->parent->children[1]->st_entry->label);
        IR.push_back(this->_3acode);
        // to pop the params
        for (int i = 0; i < this->parent->children[2]->children.size() ; i++) {
            if(this->parent->children[2]->children[i]->name == "self") {
                q = new Quadruple("", "", "", "self", Q_POP_PARAM);
            }
            else {
                q = new Quadruple("", "", "", this->parent->children[2]->children[i]->children[0]->name, Q_POP_PARAM);
            }
            IR.push_back(q);
        }
    }
    else if ((this->name == "None")) {
        this->operand_type = D_VOID;
    }
}

void node::generate_3ac() {
    //TODO: pending 3AC generation for functions, classes, for loop, return stmt, ternary operator
    //TODO: update SYMBOL_TABLE variable to manage scope (that will be used in typechecking)
    symbol_table_entry* entry;
    Quadruple* q;
    string temp_result;
    node* tmp;
    int offset;
    if(this->name == "while") { // need to store this for backpatching
        LABEL_CNT_STACK.push(LABEL_COUNTER);
    }
    if(this->type == FUNCDEF) {
        pending_return = true;
        SYMBOL_TABLE = SYMBOL_TABLE->get_entry(this->children[1]->name)->child_symbol_table; // the function name is children[1]
    }
    if (this->type == CLASSDEF) {
        pending_init = true;
        SYMBOL_TABLE = SYMBOL_TABLE->get_entry(this->children[1]->name)->child_symbol_table; // the class name is children[1]
    }
    for(auto child: this->children) {
        if(child != nullptr) {
            // cout << "before calling " << child->name << ", IC = " << INTERMEDIATE_COUNTER << endl;
            if(this->type == FILE_INPUT) {
                INTERMEDIATE_COUNTER = 1; // resetting it for each statement
                // IR.push_back(new Quadruple("", "", "", "", Q_BLANK));
            }
            child->generate_3ac();
        }
    }
    // cout << "--------" << endl;
    // cout << this->name << endl;
    // for(auto q : IR) {
    //     cout << q->code << endl;
    // }
    string op1, op2, result, op;
    switch(this->type) {
        case FILE_INPUT:
            q = new Quadruple("", "", "", "", Q_BLANK);
            IR.push_back(q);
            return;
        case KEYWORD:
            this->generate_3ac_keywords();
        case INT:
        case FLOAT:
        case BOOL:
        case STR:
        case LIST:
            return;
        case IDENTIFIER:
            entry = SYMBOL_TABLE->get_entry(this->name);
            if (!entry) {
                entry = SYMBOL_TABLE->get_entry("self." + this->name);
            }
            if (!entry) {
                // may be referring to a function/member of a class using an object.
                if(this->parent->type == TRAILER && this->parent->children[0]->type == DOT) {
                    entry = SYMBOL_TABLE->get_entry(this->parent->parent->children[0]->name); // getting the entry corresponding to the object of the class
                    entry = SYMBOL_TABLE->get_entry(entry->class_name); // getting the entry corresponding to the class name
                    entry = entry->child_symbol_table->get_entry(this->name); // trying to find the data member/member method from that class
                }
            }
            if(!entry) {
                yylineno = this->line_no;
                yyerror("UnexpectedError: Identifier entry not found.");
            }
            this->operand_type = entry->b_type;
            if(this->operand_type == D_CLASS) {
                this->class_name = entry->name;
            }
            if(this->operand_type == D_LIST && this->parent->type == ASSIGN) {
                q = new Quadruple("", to_string(entry->l_attr.list_elem_size * entry->l_attr.num_of_elems), "", this->name, Q_ALLOC);
                IR.push_back(q);
            }
            if (this->children.size() > 0) {
                if(this->children[0]->type == IDENTIFIER) {
                    entry = SYMBOL_TABLE->get_entry(this->children[0]->name);
                    if (!entry || entry->child_symbol_table == nullptr || entry->b_type != D_CLASS) { // entry->child_symbol_table (only for class declarations)
                        yyerror("TypeError: Undeclared type or erroneous type initialization.");
                    }
                    q = new Quadruple("", to_string(entry->size), "", this->name, Q_ALLOC); // allocating memory for a class object
                    IR.push_back(q);
                }
            }
            if (this->name == "__init__" && SYMBOL_TABLE->parent && SYMBOL_TABLE->parent->st_type == CLASS) {
                if(pending_init == false) {
                    yylineno = this->line_no;
                    yyerror("SemanticError: Class has multiple __init__ functions.");
                }
                pending_init = false;
            }
            return;
        case STRING_LITERAL:
            this->operand_type = D_STRING;
            return;
        case INT_NUMBER:
            this->operand_type = D_INT;
            return;
        case BOOL_NUMBER:
            this->operand_type = D_BOOL;
            return;
        case FLOAT_NUMBER:
            this->operand_type = D_FLOAT;
            return;
        case ATOM_EXPR:
            if(this->children[0]->name == "print") {
                op1 = this->get_rhs_operand();
                this->_3acode = new Quadruple("", op1, "", "", Q_PRINT);
                // TODO: any type checking here? : yes, cannot print classes, function
                IR.push_back(this->_3acode);
            }
            else if(this->children[0]->name == "self") {
                // find offset for data members. this->children[1] is trailer, its children[1] is the name of the data member
                entry = SYMBOL_TABLE->parent->get_entry("self." +this->children[1]->children[1]->name);
                if(!entry) {
                    cout << "entry not found" << endl;
                }
                offset = entry->offset;
                this->_3acode = new Quadruple("+", "addr(self)", to_string(offset), "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
                this->operand_type = entry->b_type;
                IR.push_back(this->_3acode);
            } 
            else if(this->children[0]->name == "len") { // this->children[0]->name == "range"
                yyerror("Not supporting this yet");
            }
            else if (this->children[0]->name == "range") {
                // this is the for loop
                if (this->parent->type != CONDITION || !this->parent->parent || this->parent->parent->name != "for") {
                    yylineno = this->line_no;
                    yyerror("`range` keyword to be used in context of a for loop");
                }
                if (this->children[1]->children.size() == 1) {
                    // the case of range(a)
                    // initialize the loop-counter with 0
                    this->_3acode = new Quadruple("=", to_string(0), "", this->parent->children[0]->name, Q_ASSIGN);
                    IR.push_back(this->_3acode);
                } else if (this->children[1]->children.size() == 2) {
                    op1 = this->children[1]->get_lhs_operand();
                    if(this->children[1]->children[0]->operand_type != D_INT) {
                        yylineno = this->line_no;
                        yyerror("TypeError: `range` can only iterate over integers.");
                    }
                    this->_3acode = new Quadruple("=", op1, "", this->parent->children[0]->name, Q_ASSIGN);

                    // this->check_operand_type_compatibility(); //TODO: check that the type is integer
                    IR.push_back(this->_3acode);
                } // TODO: support range(start, stop, step) 
                else {
                    yylineno = this->line_no;
                    yyerror("IncompatiblityError: `range` accepts upto 2 arguments.");
                }
            }
            else if(this->children[0]->type == IDENTIFIER) {
                entry = SYMBOL_TABLE->get_entry(this->children[0]->name);
                if(entry->b_type == D_LIST) {
                    op1 = this->get_lhs_operand();
                    op2 = this->get_rhs_operand();
                    entry = SYMBOL_TABLE->get_entry(op1);
                    if(this->children[1]->children[0]->type == INT_NUMBER && atoi(op2.c_str()) > entry->l_attr.num_of_elems) {
                        yylineno = this->line_no;
                        yyerror(("IndexError: list index out of range for list " + this->children[0]->name).c_str());
                    }

                    // manual type checking
                    temp_result = op2;
                    if(this->children[1]->children[0]->operand_type == D_BOOL) {
                        temp_result = ("t" + to_string(INTERMEDIATE_COUNTER++));
                        if(this->children[1]->children[0]->_3acode != nullptr) {
                            // there is some temporary result stored, that has to be coerced
                            q = new Quadruple("=", "int", this->children[1]->children[0]->_3acode->result, temp_result, Q_COERCION);
                        } else {
                            q = new Quadruple("=", "int", this->children[1]->children[0]->name, temp_result, Q_COERCION);
                        }
                        this->children[1]->children[0]->operand_type = D_INT;
                        IR.push_back(q);
                    }
                    else if(this->children[1]->children[0]->operand_type != D_INT) {
                        yylineno = this->children[0]->line_no;
                        yyerror("IndexError: list index must be an integer");
                    }
                    q = new Quadruple("*", temp_result, to_string(entry->l_attr.list_elem_size), "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
                    IR.push_back(q);
                    q = new Quadruple("+", "addr(" + op1 + ")", q->result, "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
                    IR.push_back(q);
                    this->_3acode = q;
                } 
                else if(entry->b_type == D_FUNCTION) {
                    this->operand_type = entry->f_attr.return_type;
                    entry = get_compatible_function_and_push_param(this);
                    q = new Quadruple("", ("+" + to_string(entry->size)).c_str(), "", "", Q_SP_UPDATE);
                    IR.push_back(q);
                    this->_3acode = new Quadruple("", entry->label, to_string(this->children[1]->children.size()), "t" + to_string(INTERMEDIATE_COUNTER++), Q_FUNC_CALL);
                    IR.push_back(this->_3acode);
                    q = new Quadruple("", ("-" + to_string(entry->size)).c_str(), "", "", Q_SP_UPDATE);
                    IR.push_back(q);
                }
                else if(entry->b_type == D_CLASS) {
                    if(entry->child_symbol_table != nullptr) {
                        // only a class declaration will have a child_symbol_table, objects of that class will not
                        this->operand_type = D_CLASS;
                        this->class_name = entry->name;
                        // push parameters of the __init__ function and call it
                        entry = call_class_init(this, entry->child_symbol_table);
                        q = new Quadruple("", ("+" + to_string(entry->size)).c_str(), "", "", Q_SP_UPDATE);
                        IR.push_back(q);
                        this->_3acode = new Quadruple("", entry->label, to_string(this->children[1]->children.size() + 1), "t" + to_string(INTERMEDIATE_COUNTER++), Q_FUNC_CALL);
                        IR.push_back(this->_3acode);
                        q = new Quadruple("", ("-" + to_string(entry->size)).c_str(), "", "", Q_SP_UPDATE);
                        IR.push_back(q);
                    } else {
                        entry = SYMBOL_TABLE->get_entry(entry->class_name); // getting the entry corresponding to the class name
                        tmp = this->children[1]->children[1]; // this points to the name of the function called from the class obj
                        entry = call_class_member_method(this, entry->child_symbol_table); // this->operand_type is set within this function itself
                        q = new Quadruple("", ("+" + to_string(entry->size)).c_str(), "", "", Q_SP_UPDATE);
                        IR.push_back(q);
                        this->_3acode = new Quadruple("", entry->label, to_string(this->children[2]->children.size() + 1), "t" + to_string(INTERMEDIATE_COUNTER++), Q_FUNC_CALL);
                        IR.push_back(this->_3acode);
                        q = new Quadruple("", ("-" + to_string(entry->size)).c_str(), "", "", Q_SP_UPDATE);
                        IR.push_back(q);
                    }
                }
            }
            return;
        case UNARY_OP:
            op1 = this->get_lhs_operand();
            this->operand_type = this->children[0]->operand_type;
            result = "t" + to_string(INTERMEDIATE_COUNTER++);
            this->_3acode = new Quadruple(this->name, op1, "", result, Q_UNARY);
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        case ASSIGN:
            this->operand_type = this->children[0]->operand_type;
            if(this->children[0]->type == IDENTIFIER && SYMBOL_TABLE->get_entry(this->children[0]->name)->b_type == D_LIST && this->children[1]->type == ASSIGN) {
                yylineno = this->line_no;
                yyerror("VersionError: multiple list assignment not supported yet. Please check for a later version.");
            }
            if(this->children[1]->type == TESTLIST_COMP) {
                do_list_assignment(this);
            }
            else {
                op1 = this->get_lhs_operand();
                op2 = this->get_rhs_operand();
                this->_3acode = new Quadruple(this->name, op2, "", op1, Q_ASSIGN);
                this->check_operand_type_compatibility();
                IR.push_back(this->_3acode);
            }
            return;
        case BIN_OP:
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            this->operand_type = max_operand_type(this->children[0]->operand_type, this->children[1]->operand_type);
            result = "t" + to_string(INTERMEDIATE_COUNTER++);
            this->_3acode = new Quadruple(this->name, op1, op2, result, Q_BINARY);
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        case BOOL_OP:
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            this->operand_type = D_BOOL;
            result = "t" + to_string(INTERMEDIATE_COUNTER++);
            this->_3acode = new Quadruple(this->name, op1, op2, result, Q_BINARY);
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        case COMPARE:
            // TODO: to support `in`, `is` etc.
            // TODO: to support string comparison
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            this->operand_type = D_BOOL;
            result = "t" + to_string(INTERMEDIATE_COUNTER++);
            this->_3acode = new Quadruple(this->name, op1, op2, result, Q_BINARY);
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        case AUGASSIGN:
            op1 = this->get_lhs_operand();
            op2 = this->get_rhs_operand();
            this->operand_type = this->children[0]->operand_type;
            result = op1;
            this->_3acode = new Quadruple(this->name.substr(0, this->name.length() - 1), op1, op2, result, Q_BINARY); // removing "=" from the name
            this->check_operand_type_compatibility();
            IR.push_back(this->_3acode);
            return;
        case CONDITION:
            if ((this->parent->name == "if") || (this->parent->name == "elif") || (this->parent->name == "while")) {
                // LABEL_CNT_STACK.push(LABEL_COUNTER++);
                if (this->children[0]->_3acode == nullptr) {
                    this->_3acode = new Quadruple("", this->children[0]->name, "", "", Q_COND_JUMP); // backpatching 2A
                } else {
                    this->_3acode = new Quadruple("", this->children[0]->_3acode->result, "", "", Q_COND_JUMP); // backpatching 2B
                }
                IR.push_back(this->_3acode);
            } else if (this->parent->name == "for") {
                if (this->children[0]->type != IDENTIFIER) {
                    yyerror("SyntaxError: Expressions cannot be assigned.");
                }
                if (this->children[2]->children[0]->type != KEYWORD || this->children[2]->children[0]->name != "range") {
                    yyerror("IncompatiblityError: `for` loop support of the form `for <var> in range(a, b){range(a)}");
                }

                if (this->children[2]->children[1]->children.size() == 1) {
                    op2 = this->children[2]->children[1]->get_lhs_operand();
                    if(this->children[2]->children[1]->children[0]->operand_type != D_INT) {
                        yylineno = this->line_no;
                        yyerror("TypeError: `range` can only iterate over integers.");
                    }
                    q = new Quadruple("<", this->children[0]->name, op2, "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
                    IR.push_back(q);
                    LABEL_CNT_STACK.push(atoi((q->label).c_str()));
                } else {
                    op2 = this->children[2]->children[1]->get_rhs_operand();
                    if(this->children[2]->children[1]->children[1]->operand_type != D_INT) {
                        yylineno = this->line_no;
                        yyerror("TypeError: `range` can only iterate over integers.");
                    }
                    q = new Quadruple("<", this->children[0]->name, op2, "t" + to_string(INTERMEDIATE_COUNTER++), Q_BINARY);
                    IR.push_back(q);
                    LABEL_CNT_STACK.push(atoi((q->label).c_str()));
                }
                this->_3acode = new Quadruple("", q->result, "", "", Q_COND_JUMP);
                IR.push_back(this->_3acode);
            }
            return;
        case SUITE:
            if (this->parent->name != "else" && this->parent->type != FUNCDEF && this->parent->type != CLASSDEF) {
                if (this->parent->name == "for") {
                    q = new Quadruple("+", this->parent->children[0]->children[0]->name, to_string(1), this->parent->children[0]->children[0]->name, Q_BINARY);
                    IR.push_back(q);
                }
                this->_3acode = new Quadruple("goto", "", "", "", Q_JUMP); // result will be renamed later (backpatching 1)
                IR.push_back(this->_3acode);
            }
            return;
        case IF_STMT:
            // we need to backpatch the labels at the ends of the body of if and elif: an unconditional jump to the end of the if-elif-else block
            for(node* child: this->children) {
                if(child->name == "if" || child->name == "elif") {
                    if(child->children[1]->type != SUITE) {
                        yyerror("UnexpectedError: Expected a suite after if/elif");
                    }
                    child->children[1]->_3acode->rename_attribute(RESULT, to_string(LABEL_COUNTER)); // (backpatching 1)
                }
            }
            return;
        case RETURN_STMT:
            tmp = this;
            while(tmp && tmp->type != FUNCDEF)
                tmp = tmp->parent;
            check_type_and_gen_3ac_return_stmt(tmp, this);
            return;
        case FUNCDEF:
            if(pending_return) {
                yylineno = this->line_no;
                if(this->children[4]->name != "None") {
                    yyerror(("SyntaxError: Function " + this->children[1]->name + " should return a value").c_str());
                }
                else {
                    // create a Quadruple to simply return from the function
                    q = new Quadruple("return", "", "", "", Q_JUMP);
                    IR.push_back(q);
                }
            }
            q = new Quadruple("", "end_func", this->children[1]->st_entry->label, "", Q_FUNC_LABEL); // inserting a blank statement at the end of each function
            IR.push_back(q);
            SYMBOL_TABLE = SYMBOL_TABLE->parent;
            return;
        case CLASSDEF:
            if (pending_init) {
                yylineno = this->line_no;
                yyerror(("SyntaxError: Class " + this->children[1]->name +  " requires an __init__ function.").c_str());
            }
            SYMBOL_TABLE = SYMBOL_TABLE->parent; // need to go back to GLOBAL scope.
            return;
        default:
            // yyerror("Unexpected error: generate_3ac called on a non-terminal node");
            return;
    }
}