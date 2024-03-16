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
    // OR,
    // AND,
    UNARY_OP,
    // NOT,

    // datatypes
    INT,
    FLOAT,
    STR,
    BOOL,
    LIST,

    // misc
    IDENTIFIER,
    STRING_LITERAL,
    // NUMBER,
    INT_NUMBER,
    FLOAT_NUMBER,

    // operators
    // PLUS,
    // MINUS,
    // STAR,
    // SLASH,
    // PERCENT,
    // DOUBLE_SLASH,
    // DOUBLE_STAR,
    // AMPER,
    // VBAR,
    // CIRCUMFLEX,
    // TILDE,
    // LEFT_SHIFT,
    // RIGHT_SHIFT,
    BIN_OP,
    COMPARE,
    ASSIGN,
    AUGASSIGN,

    // delimiters
    // LPAR,
    // RPAR,
    // LSQB,
    // RSQB,
    // LBRACE,
    // RBRACE,
    // COMMA,
    // COLON,
    // DOT,
    // SEMICOLON,
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
};

const map<node_type, string> type_map = {
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
    // {NUMBER, "NUMBER"},
    {INT_NUMBER, "INT_NUMBER"},
    {FLOAT_NUMBER, "FLOAT_NUMBER"},

    {BIN_OP, "BIN_OP"},
    {COMPARE, "COMPARE"},
    {ASSIGN, "ASSIGN"},
    {AUGASSIGN, "AUGASSIGN"},
    {DELIMITER, "DELIMITER"},
    {RARROW, "RARROW"},
};

typedef struct node {
    unsigned long long int ID; // ID for a node
    node_type type;
    string name = ""; // stores the lexeme in case of terminal, empty o.w.
    bool is_terminal = false;

    // the value of the node (assuming the primitive types are string, int, double, bool)
    string str_val = "";
    long long int int_val = 0;
    double double_val = 0.0;
    bool bool_val = false;
    string val_repr = ""; // the string representation of the value

    // line no
    int line_no = 0; // stores the line no. on which the node ends (significant only for multiline statements eg. funcdef or multiline strings)

    // parent and children
    vector<node*> children;
    node* parent = NULL;

    // linking the nodes with symbol table and symbol table entries
    struct symbol_table* st = nullptr;
    struct symbol_table_entry* st_entry = nullptr;

    // METHODS
    // constructor
    node(node_type type, string name, bool is_terminal, node* parent);
    // functional overloading to assign value to the node
    void assign_value(string value);
    void assign_value(long long int value);
    void assign_value(double value);
    void assign_value(bool value);
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

    // symbol table handling

    void set_list_attributes(struct node* annassign);
    void setup_new_st_env();
    void create_block_st(const char* block_name);   // to create symbol_table for a block scope
                                                    // only called when a block has a declaration within it
    void create_func_st();     // to create symbol_table for a function scope
    void exit_from_func(); // sets the arguments and return type. Also, restores SYMBOL_TABLE and OFFSET

    void create_class_st();
    void handle_inheritance(struct node* optional_arglist);
    void exit_from_class();

    // 3AC code
    void generate_3ac();
    string get_lhs_operand(); // to be used from node "ASSIGN" after processing of the tree
    string get_rhs_operand(); // to be used from node "ASSIGN" after processing of the tree
} node;

void prune_custom_nodes(node* parent, node* child);

void to_ast_operator(node* root, bool is_left_associative, set<string> matching_strings);

void comp_op_processing(node* root);


node* sem_lval_check(node* root);

base_data_type sem_rval_check(symbol_table* st, node* root);

void check_declare_before_use(symbol_table* st, node* root);

void add_class_st_entry(node* test, base_data_type b_type);

#endif