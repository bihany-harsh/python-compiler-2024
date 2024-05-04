// to define the node structure used in the Abstract Syntax Tree (AST)
#ifndef NODE_HPP
#define NODE_HPP
#include <string>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "symbol_table.hpp"
#include "_3AC.hpp"
using namespace std;

typedef enum {
    // keywords
    KEYWORD,
    BOOL_OP,
    UNARY_OP,

    // datatypes
    INT,
    FLOAT,
    STR,
    BOOL,
    LIST,

    // misc
    IDENTIFIER,
    STRING_LITERAL,
    BOOL_NUMBER, // True or False
    INT_NUMBER,
    FLOAT_NUMBER,

    // operators
    BIN_OP,
    COMPARE,
    ASSIGN,
    AUGASSIGN,

    // delimiters
    DOT,
    DELIMITER,
    RARROW,


    //non-terminals
    FILE_INPUT,
    MULTIPLE_LINES,
    SINGLE_LINE,
    SIMPLE_STMT,
    MANY_SMALL_STMTS,

    EXPR_STMT,
    MANY_EQUAL_TEST,
    ANNASSIGN,
    OPTIONAL_ASSIGN_TEST,
    TESTLIST,
    MANY_COMMA_TOK_TEST,

    CONDITION, // for loop/if-elif conditions
    TEST,
    OPTIONAL_IF_ELSE,
    OR_TEST,
    MANY_OR_TOK_AND_TEST,
    AND_TEST,
    MANY_AND_TOK_NOT_TEST,
    NOT_TEST,
    COMPARISON,
    MANY_COMPARISON_EXPR,
    COMP_OP,

    EXPR,
    MANY_VBAR_TOK_XOR_EXPR,
    XOR_EXPR,
    MANY_CFLEX_TOK_AND_EXPR,
    AND_EXPR,
    MANY_AMPER_TOK_SHIFT_EXPR,
    SHIFT_EXPR,
    MANY_SHIFT_OP_ARITH_EXPR,
    ARITH_EXPR,
    MANY_ARITH_EXPR,

    TERM,
    MANY_MOD_FACTOR,
    FACTOR,
    POWER,
    OPTIONAL_DSTAR_TOK_FACTOR,

    ATOM_EXPR,
    ATOM,
    AT_LEAST_ONE_STRING,
    MANY_TRAILERS,
    TRAILER,

    ARGLIST,
    MANY_COMMA_ARGUMENT,
    ARGUMENT,
    SUBSCRIPTLIST,
    MANY_COMMA_SUBSCRIPT,

    COMP_FOR,
    EXPRLIST,
    MANY_COMMA_EXPR,
    COMP_IF,
    TESTLIST_COMP,

    RETURN_STMT,
    GLOBAL_STMT,
    MANY_COMMA_TOK_IDENTIFIER,
    NONLOCAL_STMT,

    IF_STMT,
    MANY_ELIF_STMTS,
    ELSE_STMT,
    SUITE,
    AT_LEAST_ONE_STMT,
    WHILE_STMT,
    OPTIONAL_ELSE_SUITE,
    FOR_STMT,
    FUNCDEF,
    OPTIONAL_TOK_RARROW_TEST,
    PARAMETERS,
    TYPEDARGSLIST,
    OPTIONAL_EQUAL_TEST,
    MANY_COMMA_TFPDEF_OPTIONAL_EQUAL_TEST,
    TFPDEF,
    OPTIONAL_TOK_COLON_TEST,
    CLASSDEF,
    OPTIONAL_PAREN_ARGLIST,
} node_type;

const set<int> operator_set = {
    BIN_OP,
    COMPARE,
    ASSIGN,
    AUGASSIGN,
};

const set<int> unary_ops = { // it is declared so that these nodes are retained when cleaning the tree
    UNARY_OP,
    KEYWORD,
    EXPR_STMT,
    RETURN_STMT,
    PARAMETERS,
    TESTLIST_COMP,
    ANNASSIGN,
    OPTIONAL_PAREN_ARGLIST,
    SUITE,
    CONDITION, // we need it for 3AC generation so shouldn't be deleted
    IF_STMT,
    WHILE_STMT,
    FOR_STMT,
    TRAILER, // for function arguments passed while calling the function
};

const map<node_type, string> type_map = {
    {FILE_INPUT, "FILE_INPUT"},
    {KEYWORD, "KEYWORD"},
    {BOOL_OP, "BOOL_OP"},
    {UNARY_OP, "UNARY_OP"},
    {INT, "INT"},
    {FLOAT, "FLOAT"},
    {STR, "STR"},
    {BOOL, "BOOL"},
    {LIST, "LIST"},

    // misc
    {IDENTIFIER, "IDENTIFIER"},
    {STRING_LITERAL, "STRING_LITERAL"},
    {BOOL_NUMBER, "BOOL_NUMBER"},
    {INT_NUMBER, "INT_NUMBER"},
    {FLOAT_NUMBER, "FLOAT_NUMBER"},

    {BIN_OP, "BIN_OP"},
    {COMPARE, "COMPARE"},
    {ASSIGN, "ASSIGN"},
    {AUGASSIGN, "AUGASSIGN"},
    {DELIMITER, "DELIMITER"},
    {DOT, "DOT"},
    {RARROW, "RARROW"},
};

typedef struct node {
    unsigned long long int ID; // ID for a node
    node_type type;
    string name = ""; // stores the lexeme in case of terminal, empty o.w.
    bool is_terminal = false;

    // line no
    int line_no = 0; // stores the line no. on which the node ends (significant only for multiline statements eg. funcdef or multiline strings)

    // parent and children
    vector<node*> children;
    node* parent = NULL;

    // linking the nodes with symbol table and symbol table entries
    struct symbol_table* st = nullptr;
    struct symbol_table_entry* st_entry = nullptr;

    // Used for generating 3ac code
    Quadruple* _3acode = nullptr;
    base_data_type operand_type = D_VOID; // storing the data type of the operand, to check type compatibility
    vector<node*> break_nodes; // to be used only for 'for' and 'while' nodes
    vector<node*> continue_nodes; // to be used only for 'for' and 'while' nodes
    string class_name = ""; // to be used only when we have a class node/ list of classes

    // METHODS
    // constructor
    node(node_type type, string name, bool is_terminal, node* parent);
    // add a child
    void add_parent_child_relation(node* child);
    void handle_annassign();  

    // tree traversal
    void traverse_tree(); // recommended to be used only from root

    // tree cleaning
    void clean_tree();  // recommended to be used only from root
    bool delete_delimiters(); // recommended to be used only from root
    void delete_single_child_nodes(); // recommended to be used only from root

    // to parse tree
    void generate_dot_script();
    void check_multi_assignment();
    void update_list_param();

    // symbol table handling

    void set_list_attributes(struct node* annassign);
    void setup_new_st_env();
    void create_block_st(const char* block_name);   // to create symbol_table for a block scope
                                                    // only called when a block has a declaration within it
    void create_func_st();     // to create symbol_table for a function scope
    void set_function_attr_and_return();    // to set fattr of args and return and also to add st_entry to the parent symbol table
    void exit_from_func(); // sets the arguments and return type. Also, restores SYMBOL_TABLE and OFFSET

    void create_class_st();
    void handle_inheritance(struct node* optional_arglist);
    void exit_from_class();

    // 3AC code
    void generate_3ac();
    void generate_3ac_keywords();
    string get_lhs_operand(); // to be used from node "ASSIGN" after processing of the tree
    string get_rhs_operand(); // to be used from node "ASSIGN" after processing of the tree
    void check_operand_type_compatibility();
    void add_break_node(node* break_node);
    void add_continue_node(node* continue_node);
} node;

void prune_custom_nodes(node* parent, node* child);

void to_ast_operator(node* root, bool is_left_associative, set<string> matching_strings);

void comp_op_processing(node* root);


node* sem_lval_check(node* root, int is_declared);

string get_class_name(node* test);

base_data_type sem_rval_check(symbol_table* st, node* root);

string get_base_type_of_list(node* test);

void check_declare_before_use(symbol_table* st, node* root);

void find_return_stmts_and_set_return_list_attr(node* suite, st_entry* func_entry);

void set_list_elem_type(symbol_table* st, node* test, st_entry* new_entry);

void add_class_st_entry(node* test, base_data_type b_type);

void do_list_assignment(node* assign);

base_data_type max_operand_type(base_data_type type1, base_data_type type2);

node* find_loop_ancestor(node* root); // root would be a node pointing to "break" or "continue"

string make_function_label(node* funcdef);

pair<bool,bool> check_coerce_required(pair<base_data_type, string> formal, pair<base_data_type, string> actual);

st_entry* find_class_function_entry(string class_name, node* atom_expr);

symbol_table_entry* call_class_init(node* atom_expr, symbol_table* class_st);

symbol_table_entry* get_compatible_function_and_push_param(node* atom_expr);

void check_type_and_gen_3ac_return_stmt(node* funcdef, node* return_stmt);
#endif