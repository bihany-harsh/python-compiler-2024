// to define the node structure used in the Abstract Syntax Tree (AST)
#include <string>
#include <vector>
#include <set>
using namespace std;

typedef enum {
    // keywords
    FALSE,
    TRUE,
    NONE,
    AND,
    BREAK,
    CLASS,
    CONTINUE,
    DEF,
    ELIF,
    ELSE,
    FOR,
    GLOBAL,
    IF,
    IN,
    IS,
    NON_LOCAL,
    NOT,
    OR,
    PASS,
    WHILE,
    RETURN,

    // datatypes
    INT,
    FLOAT,
    STR,
    BOOL,

    // misc
    IDENTIFIER,
    STRING_LITERAL,
    NUMBER,

    // operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    DOUBLE_SLASH,
    DOUBLE_STAR,

    EQ_EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,

    AMPER,
    VBAR,
    CIRCUMFLEX,
    TILDE,
    LEFT_SHIFT,
    RIGHT_SHIFT,

    EQUAL,
    PLUS_EQUAL,
    MINUS_EQUAL,
    STAR_EQUAL,
    SLASH_EQUAL,
    DOUBLE_SLASH_EQUAL,
    PERCENT_EQUAL,
    DOUBLE_STAR_EQUAL,
    AMPER_EQUAL,
    VBAR_EQUAL,
    CIRCUMFLEX_EQUAL,
    LEFT_SHIFT_EQUAL,
    RIGHT_SHIFT_EQUAL,

    // delimiters
    LPAR,
    RPAR,
    LSQB,
    RSQB,
    LBRACE,
    RBRACE,
    COMMA,
    COLON,
    DOT,
    SEMICOLON,
    RARROW,


    //non-terminals
    FILE_INPUT,
    MULTIPLE_LINES,
    SINGLE_LINE,
    SIMPLE_STMT,
    MANY_SMALL_STMTS,

    EXPR_STMT,
    MANY_EQUAL_TESTLIST_STAR_EXPR,
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

const vector<int> delimiter_vector = {
    LPAR,
    RPAR,
    LSQB,
    RSQB,
    LBRACE,
    RBRACE,
    COLON,
    DOT,
    SEMICOLON,
};

const set<int> operator_set = {
    PLUS,
    MINUS,
    STAR,
    SLASH,
    PERCENT,
    DOUBLE_SLASH,
    DOUBLE_STAR,

    EQ_EQUAL,
    NOT_EQUAL,
    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,

    AMPER,
    VBAR,
    CIRCUMFLEX,
    TILDE,
    LEFT_SHIFT,
    RIGHT_SHIFT,

    EQUAL,
    PLUS_EQUAL,
    MINUS_EQUAL,
    STAR_EQUAL,
    SLASH_EQUAL,
    DOUBLE_SLASH_EQUAL,
    PERCENT_EQUAL,
    DOUBLE_STAR_EQUAL,
    AMPER_EQUAL,
    VBAR_EQUAL,
    CIRCUMFLEX_EQUAL,
    LEFT_SHIFT_EQUAL,
    RIGHT_SHIFT_EQUAL,
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

    // tree traversal
    void traverse_tree(); // recommended to be used only from root

    // tree cleaning
    void clean_tree();  // recommended to be used only from root
    bool delete_delimiters(); // recommended to be used only from root
    void delete_single_child_nodes(); // recommended to be used only from root

    // to parse tree
    void generate_dot_script();

    //
} node;

void prune_custom_nodes(node* parent, node* child);

void ast_conv_operators(node* root);

void clear_delete_nodes();
