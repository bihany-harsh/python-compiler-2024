%{
    #include <iostream>
    #include <stack>
    #include <cstring>
    #include <algorithm>
    #include <set>
    #include "include/node.hpp"
    using namespace std;

    extern FILE* yyin;
    extern int yylineno;
    extern bool verbose_flag;
    extern stack<int> INDENT_STACK;

    char compound_stmt_type[64];
    char error_string[256];
    extern node* AST_ROOT;
    // TODO: test with optional_if_else (body with head of production = test)

    extern char* yytext;

    int yylex(void);
    void yyerror (char const *s) {
        fprintf (stdout, "line number %d: \t %s\n", yylineno, s);
        exit(-1);
    }

    int join_lines_implicitly = 0;

    // TODO: funcdef ")" error check
%}

%union{
    char* str_val;
    struct node* tree_node;
}

%token<tree_node> TOK_IDENTIFIER TOK_NUMBER TOK_STRING_LITERAL

%token<tree_node> TOK_NEWLINE TOK_INDENT TOK_DEDENT

%token<tree_node> TOK_FALSE TOK_NONE TOK_TRUE
%token<tree_node> TOK_AND TOK_OR TOK_NOT
%token<tree_node> TOK_BREAK TOK_CONTINUE TOK_RETURN
%token<tree_node> TOK_GLOBAL TOK_NON_LOCAL TOK_DEF TOK_CLASS
%token<tree_node> TOK_IF TOK_ELSE TOK_ELIF
%token<tree_node> TOK_IN TOK_IS
%token<tree_node> TOK_PASS
%token<tree_node> TOK_FOR TOK_WHILE

%token<tree_node> TOK_INT TOK_FLOAT TOK_BOOL TOK_STR

%token<tree_node> TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_PERCENT TOK_DOUBLE_SLASH TOK_DOUBLE_STAR

%token<tree_node> TOK_EQ_EQUAL TOK_NOT_EQUAL TOK_GREATER TOK_LESS TOK_GREATER_EQUAL TOK_LESS_EQUAL

%token<tree_node> TOK_AMPER TOK_VBAR TOK_CIRCUMFLEX TOK_TILDE TOK_LEFT_SHIFT TOK_RIGHT_SHIFT

%token<tree_node> TOK_EQUAL TOK_PLUS_EQUAL TOK_MINUS_EQUAL TOK_STAR_EQUAL TOK_SLASH_EQUAL TOK_PERCENT_EQUAL
%token<tree_node> TOK_AMPER_EQUAL TOK_VBAR_EQUAL TOK_CIRCUMFLEX_EQUAL TOK_LEFT_SHIFT_EQUAL TOK_RIGHT_SHIFT_EQUAL TOK_DOUBLE_STAR_EQUAL TOK_DOUBLE_SLASH_EQUAL

%token<tree_node> TOK_LPAR TOK_RPAR TOK_LSQB TOK_RSQB TOK_LBRACE TOK_RBRACE
%token<tree_node> TOK_SEMICOLON TOK_COLON TOK_COMMA TOK_DOT
%token<tree_node> TOK_RARROW

%type<tree_node> file_input multiple_lines single_line stmt simple_stmt small_stmt many_small_stmts
%type<tree_node> expr_stmt many_equal_test annassign optional_assign_test augassign testlist many_comma_tok_test optional_comma
%type<tree_node> test optional_if_else or_test many_or_tok_and_test and_test many_and_tok_not_test not_test comparison many_comparison_expr comp_op
%type<tree_node> expr many_vbar_tok_xor_expr xor_expr many_cflex_tok_and_expr and_expr many_amper_tok_shift_expr shift_expr many_shift_op_arith_expr arith_expr many_arith_term
%type<tree_node> term many_mod_factor factor power optional_dstar_tok_factor atom_expr atom data_type at_least_one_string many_trailers trailer optional_arglist arglist
%type<tree_node> many_comma_argument argument subscriptlist subscript many_comma_subscript optional_comp_for comp_for exprlist many_comma_expr optional_comp_iter comp_iter
%type<tree_node> comp_if test_nocond testlist_comp pass_stmt flow_stmt break_stmt continue_stmt return_stmt optional_testlist global_stmt many_comma_tok_identifier nonlocal_stmt
%type<tree_node> compound_stmt if_stmt many_elif_stmts optional_else_stmt else_stmt suite at_least_one_stmt while_stmt optional_else_suite for_stmt funcdef optional_tok_rarrow_test parameters
%type<tree_node> optional_typedargslist typedargslist optional_equal_test many_comma_tfpdef_optional_equal_test tfpdef optional_tok_colon_test classdef optional_paren_arglist

%expect 1

%%

file_input                  :   multiple_lines { 
                                    $$ = $1; 
                                    $$->name = "FILE_INPUT";
                                    AST_ROOT = $$;
                                    AST_ROOT->clean_tree();
                                    if(verbose_flag) {
                                        cout << "AST cleaning done!" << endl;
                                    }
                                    AST_ROOT->delete_delimiters();
                                    AST_ROOT->delete_single_child_nodes();
                            }
                            ;
multiple_lines              :   multiple_lines single_line {
                                    $$ = new node(MULTIPLE_LINES, "MULTIPLE_LINES", false, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1); // adds all non-null multiple_lines children of $1 to $$ and deletes $1
                                    $$->add_parent_child_relation($2);
                            }
                            |   {  $$ = NULL;  }
                            ;
single_line                 :   stmt {
                                    $$ = $1;
                            }
                            |   TOK_NEWLINE {
                                    $$ = NULL; // this corresponds to an empty line so NULL
                            }
                            ;
stmt                        :   simple_stmt {
                                    $$ = $1;
                            }
                            |   compound_stmt {
                                    $$ = $1;
                            }
                            ;
simple_stmt                 :   indent_check_small_stmt small_stmt many_small_stmts optional_semicolon TOK_NEWLINE {
                                    $$ = new node(SIMPLE_STMT, "SIMPLE_STMT", false, NULL); // children 1, 4 & 5 not needed
                                    $$->add_parent_child_relation($2);
                                    // $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                                    // $$->add_parent_child_relation($4);
                                    // prune_custom_nodes($$, $4); // $4 is not needed
                            }
                            ;
optional_semicolon          :   TOK_SEMICOLON
                            |
                            ;
small_stmt                  :   expr_stmt   {
                                    $$ = $1;
                            }
                            |   pass_stmt   {
                                    $$ = $1;
                            }
                            |   flow_stmt   {
                                    $$ = $1;
                            }
                            |   global_stmt   {
                                    $$ = $1;
                            }
                            |   nonlocal_stmt   {
                                    $$ = $1;
                            }
                            ;
indent_check_small_stmt     :   TOK_INDENT  {   snprintf(error_string, sizeof(error_string), "unexpected indent");
                                                yyerror(error_string);
                                            }
                            |
                            ;
many_small_stmts            :   many_small_stmts TOK_SEMICOLON small_stmt {
                                    $$ = new node(MANY_SMALL_STMTS, "MANY_SMALL_STMTS", false, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    // $2 = new node(SEMICOLON, ";", true, NULL);
                                    // $$->add_parent_child_relation($2); // $2 is not needed
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
expr_stmt                   :   test annassign {
                                    $$ = new node(EXPR_STMT, "EXPR_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   test augassign testlist {
                                    $$ = new node(EXPR_STMT, "EXPR_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    // set<node_type> ops = { AUGASSIGN };
                                    set<string> match = {"+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "**=", "//="};
                                    to_ast_operator($$, false, match);
                            }
                            |   test many_equal_test {
                                    $$ = new node(EXPR_STMT, "EXPR_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { ASSIGN };
                                    set<string> match = {"="};
                                    to_ast_operator($$, false, match);
                            }
                            ;
// testlist_star_expr          :   test {
//                                     $$ = $1;
//                             }
//                             |   star_expr
//                             ;
many_equal_test             :   many_equal_test TOK_EQUAL test {
                                    $$ = new node(MANY_EQUAL_TEST, "MANY_EQUAL_TEST", false, NULL);
                                    $2 = new node(ASSIGN, "=", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                }
                            |   {   $$ = NULL;  }
                            |   many_equal_test TOK_EQUAL {
                                    snprintf(error_string, sizeof(error_string), "SyntaxError: invalid syntax");
                                    yyerror(error_string);
                            }
                            ;
annassign                   :   TOK_COLON test optional_assign_test {
                                    $$ = new node(ANNASSIGN, "ANNASSIGN", false, NULL);
                                    $1 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    // $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
optional_assign_test        :   TOK_EQUAL test {
                                    $$ = new node(OPTIONAL_ASSIGN_TEST, "OPTIONAL_ASSIGN_TEST", false, NULL);
                                    $1 = new node(ASSIGN, "=", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   {   $$ = NULL;  }
                            ;
augassign                   :   TOK_PLUS_EQUAL {
                                    $$ = new node(AUGASSIGN, "+=", true, NULL);
                            }
                            |   TOK_MINUS_EQUAL {
                                    $$ = new node(AUGASSIGN, "-=", true, NULL);
                            }
                            |   TOK_STAR_EQUAL {
                                    $$ = new node(AUGASSIGN, "*=", true, NULL);
                            }
                            |   TOK_SLASH_EQUAL {
                                    $$ = new node(AUGASSIGN, "/=", true, NULL);
                            }
                            |   TOK_PERCENT_EQUAL {
                                    $$ = new node(AUGASSIGN, "%=", true, NULL);
                            }
                            |   TOK_AMPER_EQUAL {
                                    $$ = new node(AUGASSIGN, "&=", true, NULL);
                            }
                            |   TOK_VBAR_EQUAL {
                                    $$ = new node(AUGASSIGN, "|=", true, NULL);
                            }
                            |   TOK_CIRCUMFLEX_EQUAL {
                                    $$ = new node(AUGASSIGN, "^=", true, NULL);
                            }
                            |   TOK_LEFT_SHIFT_EQUAL {
                                    $$ = new node(AUGASSIGN, "<<=", true, NULL);
                            }
                            |   TOK_RIGHT_SHIFT_EQUAL {
                                    $$ = new node(AUGASSIGN, ">>=", true, NULL);
                            }
                            |   TOK_DOUBLE_STAR_EQUAL {
                                    $$ = new node(AUGASSIGN, "**=", true, NULL);
                            }
                            |   TOK_DOUBLE_SLASH_EQUAL {
                                    $$ = new node(AUGASSIGN, "//=", true, NULL);
                            }
                            ;
testlist                    :   test many_comma_tok_test optional_comma {
                                    $$ = new node(TESTLIST, "TESTLIST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
many_comma_tok_test         :   many_comma_tok_test TOK_COMMA test {
                                    $$ = new node(MANY_COMMA_TOK_TEST, "MANY_COMMA_TOK_TEST", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL; }
optional_comma              :   TOK_COMMA {
                                    $$ = new node(DELIMITER, ",", true, NULL);
                            }
                            |   {   $$ = NULL;  }
                            ;

test                        :   or_test optional_if_else {
                                    $$ = new node(TEST, "TEST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                            }
                            ;
optional_if_else            :   TOK_IF or_test TOK_ELSE test {
                                    $$ = new node(OPTIONAL_IF_ELSE, "OPTIONAL_IF_ELSE", false, NULL);
                                    $1 = new node(KEYWORD, "if", true, NULL);
                                    $3 = new node(KEYWORD, "else", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    $1->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    // $$->add_parent_child_relation($4);
                                    $3->add_parent_child_relation($4);
                            }
                            |   {   $$ = NULL;  }
or_test                     :   and_test many_or_tok_and_test {
                                    $$ = new node(OR_TEST, "OR_TEST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { BOOL_OP };
                                    set<string> match = {"or"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_or_tok_and_test        :   many_or_tok_and_test TOK_OR and_test {
                                    $$ = new node(MANY_OR_TOK_AND_TEST, "MANY_OR_TOK_AND_TEST", false, NULL);
                                    $2 = new node(BOOL_OP, "or", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
and_test                    :   not_test many_and_tok_not_test {
                                    $$ = new node(AND_TEST, "AND_TEST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { BOOL_OP };
                                    set<string> match = {"and"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_and_tok_not_test       :   many_and_tok_not_test TOK_AND not_test {
                                    $$ = new node(MANY_AND_TOK_NOT_TEST, "MANY_AND_TOK_NOT_TEST", false, NULL);
                                    $2 = new node(BOOL_OP, "and", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
not_test                    :   TOK_NOT not_test {
                                    $$ = new node(UNARY_OP, "not", true, NULL);
                                    $$->add_parent_child_relation($2);
                            }
                            |   comparison {
                                    $$ = $1;
                            }
                            ;
comparison                  :   expr many_comparison_expr {
                                    $$ = new node(COMPARISON, "COMPARISON", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    $$->delete_single_child_nodes(); //TODO: clean this up
                                    comp_op_processing($$);
                            }
                            ;
many_comparison_expr        :   many_comparison_expr comp_op expr {
                                    $$ = new node(MANY_COMPARISON_EXPR, "MANY_COMPARISON_EXPR", false, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
comp_op                     :   TOK_LESS {
                                    $$ = new node(COMPARE, "<", true, NULL);
                            }
                            |   TOK_GREATER {
                                    $$ = new node(COMPARE, ">", true, NULL);
                            }
                            |   TOK_EQ_EQUAL {
                                    $$ = new node(COMPARE, "==", true, NULL);
                            }
                            |   TOK_GREATER_EQUAL {
                                    $$ = new node(COMPARE, ">=", true, NULL);
                            }
                            |   TOK_LESS_EQUAL {
                                    $$ = new node(COMPARE, "<=", true, NULL);
                            }
                            |   TOK_NOT_EQUAL {
                                    $$ = new node(COMPARE, "!=", true, NULL);
                            }
                            |   TOK_IN { // in has other uses (as iterator in for) but can change to COMPARE here since this is a specific production
                                    $$ = new node(COMPARE, "in", true, NULL);
                            }
                            |   TOK_NOT TOK_IN {
                                    $$ = new node(COMPARE, "not in", true, NULL);
                            }
                            |   TOK_IS {
                                    $$ = new node(COMPARE, "is", true, NULL);
                            }
                            |   TOK_IS TOK_NOT {
                                    $$ = new node(COMPARE, "is not", true, NULL);
                            }
                            ;
expr                        :   xor_expr many_vbar_tok_xor_expr {
                                    $$ = new node(EXPR, "EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { BIN_OP };
                                    set<string> match = {"|"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_vbar_tok_xor_expr      :   many_vbar_tok_xor_expr TOK_VBAR xor_expr {
                                    $$ = new node(MANY_VBAR_TOK_XOR_EXPR, "MANY_VBAR_TOK_XOR_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "|", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
xor_expr                    :   and_expr many_cflex_tok_and_expr {
                                    $$ = new node(XOR_EXPR, "XOR_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { BIN_OP };
                                    set<string> match = {"^"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_cflex_tok_and_expr     :   many_cflex_tok_and_expr TOK_CIRCUMFLEX and_expr {
                                    $$ = new node(MANY_CFLEX_TOK_AND_EXPR, "MANY_CFLEX_TOK_AND_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "^", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
and_expr                    :   shift_expr many_amper_tok_shift_expr {
                                    $$ = new node(AND_EXPR, "AND_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { AMPER };
                                    set<string> match = {"&"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_amper_tok_shift_expr   :   many_amper_tok_shift_expr TOK_AMPER shift_expr {
                                    $$ = new node(MANY_AMPER_TOK_SHIFT_EXPR, "MANY_AMPER_TOK_SHIFT_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "&", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
shift_expr                  :   arith_expr many_shift_op_arith_expr {
                                    $$ = new node(SHIFT_EXPR, "SHIFT_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { LEFT_SHIFT, RIGHT_SHIFT };
                                    set<string> match = {"<<", ">>"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_shift_op_arith_expr    :   many_shift_op_arith_expr TOK_LEFT_SHIFT arith_expr {
                                    $$ = new node(MANY_SHIFT_OP_ARITH_EXPR, "MANY_SHIFT_OP_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "<<", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_shift_op_arith_expr TOK_RIGHT_SHIFT arith_expr {
                                    $$ = new node(MANY_SHIFT_OP_ARITH_EXPR, "MANY_SHIFT_OP_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, ">>", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
arith_expr                  :   term many_arith_term {
                                    $$ = new node(ARITH_EXPR, "ARITH_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { PLUS, MINUS };
                                    set<string> match = {"+", "-"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_arith_term             :   many_arith_term TOK_PLUS term {
                                    $$ = new node(MANY_ARITH_EXPR, "MANY_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "+", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_arith_term TOK_MINUS term {
                                    $$ = new node(MANY_ARITH_EXPR, "MANY_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "-", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
term                        :   factor many_mod_factor {
                                    $$ = new node(TERM, "TERM", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = { STAR, SLASH, PERCENT, DOUBLE_SLASH };
                                    set<string> match = {"*", "/", "%", "//"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_mod_factor             :   many_mod_factor TOK_STAR factor {
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "*", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_mod_factor TOK_SLASH factor {
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "/", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_mod_factor TOK_PERCENT factor {
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "%", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_mod_factor TOK_DOUBLE_SLASH factor {
                                    // factor corresponds to sort of using unary operator +, -, ~
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "//", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
factor                      :   TOK_PLUS factor {
                                    $$ = new node(UNARY_OP, "+", true, NULL);
                                    $$->add_parent_child_relation($2);
                                    // $$->add_parent_child_relation($2);
                                    // prune_custom_nodes($$, $2);
                            }
                            |   TOK_MINUS factor {
                                    $$ = new node(UNARY_OP, "-", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    // prune_custom_nodes($$, $2);
                            }
                            |   TOK_TILDE factor {
                                    $$ = new node(UNARY_OP, "~", true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    // prune_custom_nodes($$, $2);
                            }
                            |   power {
                                    $$ = $1;
                            }
                            ;
power                       :   atom_expr optional_dstar_tok_factor {
                                    $$ = new node(POWER, "POWER", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                    // set<node_type> ops = {DOUBLE_STAR};
                                    set<string> match = {"**"};
                                    to_ast_operator($$, false, match);
                            }
                            ;
optional_dstar_tok_factor   :   TOK_DOUBLE_STAR factor {
                                    $$ = new node(OPTIONAL_DSTAR_TOK_FACTOR, "OPTIONAL_DSTAR_TOK_FACTOR", false, NULL);
                                    $1 = new node(BIN_OP, "**", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   {   $$ = NULL;  }
                            ;
atom_expr                   :   atom many_trailers {
                                    $$ = new node(ATOM_EXPR, "ATOM_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                            }
                            ;
atom                        :   TOK_LPAR { join_lines_implicitly++; } testlist_comp TOK_RPAR { 
                                    join_lines_implicitly--;
                                    $$ = new node(ATOM, "ATOM", false, NULL);
                                    $1 = new node(DELIMITER, "(", true, NULL);
                                    $4 = new node(DELIMITER, ")", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($3); 
                                    $$->add_parent_child_relation($4);
                            }
                            |   TOK_LSQB { join_lines_implicitly++; } testlist_comp TOK_RSQB { 
                                    join_lines_implicitly--;
                                    $$ = new node(ATOM, "ATOM", false, NULL);
                                    $1 = new node(DELIMITER, "[", true, NULL);
                                    $4 = new node(DELIMITER, "]", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($3); 
                                    $$->add_parent_child_relation($4);
                            }
                            |   TOK_IDENTIFIER {
                                    $$ = new node(IDENTIFIER, yytext, true, NULL);
                            }
                            |   data_type {
                                    $$ = $1;
                            }
                            |   TOK_NUMBER {
                                    $$ = new node(NUMBER, yytext, true, NULL);
                            }
                            |   at_least_one_string {
                                    $$ = $1;
                            }
                            |   TOK_NONE {
                                    $$ = new node(KEYWORD, "None", true, NULL);
                            }
                            |   TOK_TRUE {
                                    $$ = new node(KEYWORD, "True", true, NULL);
                            }
                            |   TOK_FALSE {
                                    $$ = new node(KEYWORD, "False", true, NULL);
                            }
                            ;
data_type                   :   TOK_INT {
                                    $$ = new node(INT, "int", true, NULL);
                            }
                            |   TOK_FLOAT {
                                    $$ = new node(FLOAT, "float", true, NULL);
                            }
                            |   TOK_STR {
                                    $$ = new node(STR, "str", true, NULL);
                            }
                            |   TOK_BOOL {
                                    $$ = new node(BOOL, "bool", true, NULL);
                            }
                            ;
at_least_one_string         :   at_least_one_string TOK_STRING_LITERAL {
                                    $$ = new node(AT_LEAST_ONE_STRING, "AT_LEAST_ONE_STRING", false, NULL);
                                    // the string literal is "<text>", however we need that the val_repr as well as the name is <text>
                                    string str_literal = "\\\"" + string(yytext).substr(1, strlen(yytext) - 2) + "\\\"";
                                    $2 = new node(STRING_LITERAL, str_literal.c_str(), true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   TOK_STRING_LITERAL {
                                    string str_literal = "\\\"" + string(yytext).substr(1, strlen(yytext) - 2) + "\\\"";
                                    $$ = new node(STRING_LITERAL, str_literal.c_str(), true, NULL);
                            }
                            ;
many_trailers               :   many_trailers trailer {
                                    $$ = new node(MANY_TRAILERS, "MANY_TRAILERS", false, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   {   $$ = NULL;  }
                            ;
trailer                     :   TOK_LPAR { join_lines_implicitly++; } optional_arglist TOK_RPAR { 
                                    join_lines_implicitly--;
                                    $$ = new node(TRAILER, "TRAILER", false, NULL);
                                    $1 = new node(DELIMITER, "(", true, NULL);
                                    $4 = new node(DELIMITER, ")", true, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3); 
                                    $$->add_parent_child_relation($4);
                            }
                            |   TOK_LSQB { join_lines_implicitly++; } subscriptlist TOK_RSQB { 
                                    join_lines_implicitly--;
                                    $$ = new node(TRAILER, "TRAILER", false, NULL);
                                    $1 = new node(DELIMITER, "[", true, NULL);
                                    $4 = new node(DELIMITER, "]", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    // prune_custom_nodes($$, $3);
                                    $$->add_parent_child_relation($3);
                                    $$->add_parent_child_relation($4);
                            }
                            |   TOK_DOT TOK_IDENTIFIER {
                                    $$ = new node(TRAILER, "TRAILER", false, NULL);
                                    $1 = new node(DELIMITER, ".", true, NULL);
                                    $2 = new node(IDENTIFIER, yytext, true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            ;
optional_arglist            :   arglist {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
                            ;
arglist                     :   argument many_comma_argument optional_comma {
                                    $$ = new node(ARGLIST, "ARGLIST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
many_comma_argument         :   many_comma_argument TOK_COMMA argument {
                                    $$ = new node(MANY_COMMA_ARGUMENT, "MANY_COMMA_ARGUMENT", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
argument                    :   test optional_comp_for {
                                    $$ = new node(ARGUMENT, "ARGUMENT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                            }
                            |   test TOK_EQUAL test {
                                    $$ = new node(ARGUMENT, "ARGUMENT", false, NULL);
                                    $2 = new node(ASSIGN, "=", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            ;
subscriptlist               :   subscript many_comma_subscript optional_comma {
                                    $$ = new node(SUBSCRIPTLIST, "SUBSCRIPTLIST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
subscript                   :   test {
                                    $$ = $1;
                            }
                            ;
many_comma_subscript        :   many_comma_subscript TOK_COMMA subscript {
                                    $$ = new node(MANY_COMMA_SUBSCRIPT, "MANY_COMMA_SUBSCRIPT", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
optional_comp_for           :   comp_for {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
                            ;
comp_for                    :   TOK_FOR exprlist TOK_IN or_test optional_comp_iter {
                                    $$ = new node(COMP_FOR, "COMP_FOR", false, NULL);
                                    $1 = new node(KEYWORD, "for", true, NULL);
                                    $3 = new node(KEYWORD, "in", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    $$->add_parent_child_relation($4);
                                //     $$->add_parent_child_relation($5);
                                    prune_custom_nodes($$, $5);
                            }
                            ;
exprlist                    :   expr many_comma_expr optional_comma {
                                    $$ = new node(EXPRLIST, "EXPRLIST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
many_comma_expr             :   many_comma_expr TOK_COMMA expr {
                                    $$ = new node(MANY_COMMA_EXPR, "MANY_COMMA_EXPR", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
optional_comp_iter          :   comp_iter {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
                            ;
comp_iter                   :   comp_for {
                                    $$ = $1;
                            }
                            |   comp_if {
                                    $$ = $1;
                            }
                            ;
comp_if                     :   TOK_IF test_nocond optional_comp_iter {
                                    $$ = new node(COMP_IF, "COMP_IF", false, NULL);
                                    $1 = new node(KEYWORD, "if", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
test_nocond                 :   or_test {
                                    $$ = $1;
                            }
                            ;
testlist_comp               :   test comp_for {
                                    $$ = new node(TESTLIST_COMP, "TESTLIST_COMP", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   test many_comma_tok_test optional_comma {
                                    $$ = new node(TESTLIST_COMP, "TESTLIST_COMP", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            |   {
                                    $$ = new node(TESTLIST_COMP, "TESTLIST_COMP", false, NULL);
                                }
                            ;

pass_stmt                   :   TOK_PASS {
                                    $$ = new node(KEYWORD, "pass", true, NULL);
                            }
                            ;

flow_stmt                   :   break_stmt {
                                    $$ = $1;
                            }
                            |   continue_stmt {
                                    $$ = $1;
                            }
                            |   return_stmt  {
                                    $$ = $1;
                            }
                            ;
break_stmt                  :   TOK_BREAK {
                                    $$ = new node(KEYWORD, "break", true, NULL);
                            }
                            ;
continue_stmt               :   TOK_CONTINUE {
                                    $$ = new node(KEYWORD, "continue", true, NULL);
                            }
                            ;
return_stmt                 :   TOK_RETURN optional_testlist {
                                    $$ = new node(RETURN_STMT, "RETURN_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "return", true, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                            }
                            ;
optional_testlist           :   testlist {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
                            ;
global_stmt                 :   TOK_GLOBAL TOK_IDENTIFIER { 
                                    $2 = new node(IDENTIFIER, yytext, true, NULL); } many_comma_tok_identifier {
                                    $$ = new node(GLOBAL_STMT, "GLOBAL_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "global", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($4);
                            }
                            ;
many_comma_tok_identifier   :   many_comma_tok_identifier TOK_COMMA TOK_IDENTIFIER {
                                    $$ = new node(MANY_COMMA_TOK_IDENTIFIER, "MANY_COMMA_TOK_IDENTIFIER", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                    $3 = new node(IDENTIFIER, yytext, true, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL; }
                            ;

nonlocal_stmt               :   TOK_NON_LOCAL TOK_IDENTIFIER { $2 = new node(IDENTIFIER, yytext, true, NULL); } many_comma_tok_identifier {
                                    $$ = new node(NONLOCAL_STMT, "NONLOCAL_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "nonlocal", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    // $$->add_parent_child_relation($4);
                                    prune_custom_nodes($$, $4);
                            }
                            ;

compound_stmt               :   if_stmt {
                                    $$ = $1;
                            }
                            |   while_stmt {
                                    $$ = $1;
                            }
                            |   for_stmt {
                                    $$ = $1;
                            }
                            |   funcdef {
                                    $$ = $1;
                            }
                            |   classdef {
                                    $$ = $1;
                            }
                            ;

if_stmt                     :   TOK_IF { strcpy(compound_stmt_type, "\'if\'"); } test TOK_COLON suite many_elif_stmts optional_else_stmt {
                                    $$ = new node(IF_STMT, "IF_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "if", true, NULL);
                                    $4 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $1->add_parent_child_relation($3);
                                    $1->add_parent_child_relation($4);
                                    $1->add_parent_child_relation($5);
                                //     $$->add_parent_child_relation($6);
                                    prune_custom_nodes($$, $6);
                                //     $$->add_parent_child_relation($7);
                                    prune_custom_nodes($$, $7);
                            }
                            |   TOK_IF TOK_COLON {
                                    snprintf(error_string, sizeof(error_string), "SyntaxError: invalid syntax");
                                    yyerror(error_string);
                            }
                            ;
many_elif_stmts             :   many_elif_stmts TOK_ELIF { strcpy(compound_stmt_type, "\'elif\'"); } test TOK_COLON suite {
                                    $$ = new node(MANY_ELIF_STMTS, "MANY_ELIF_STMTS", false, NULL);
                                    $2 = new node(KEYWORD, "elif", true, NULL);
                                    $5 = new node(DELIMITER, ":", true, NULL);
                                //     $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $2->add_parent_child_relation($4);
                                    $2->add_parent_child_relation($5);
                                    $2->add_parent_child_relation($6);
                            }
                            |   {   $$ = NULL;  }
optional_else_stmt          :   else_stmt {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
else_stmt                   :   TOK_ELSE { strcpy(compound_stmt_type, "\'else\'"); } TOK_COLON suite {
                                    $$ = new node(ELSE_STMT, "ELSE_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "else", true, NULL);
                                    $3 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $1->add_parent_child_relation($3);
                                    $1->add_parent_child_relation($4);
                            }
                            ;
suite                       :   simple_stmt {
                                    $$ = $1;
                                    $$->type = SUITE;
                                    $$->name = "SUITE";
                            }
                            |   TOK_NEWLINE indent_check_compound at_least_one_stmt TOK_DEDENT {
                                    $$ = $3; // note that NEWLINE, INDENT and DEDENT are not needed for semantics
                                    $$->type = SUITE;
                                    $$->name = "SUITE";
                            }
                            ;
indent_check_compound       :   TOK_INDENT
                            |   {
                                    snprintf(error_string, sizeof(error_string), "IndentationFault: expected an indented block after %s", compound_stmt_type);
                                    yyerror(error_string);
                                }
                            ;
at_least_one_stmt           :   at_least_one_stmt stmt {
                                    $$ = new node(AT_LEAST_ONE_STMT, "AT_LEAST_ONE_STMT", false, NULL);
                                    // $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   stmt {
                                    $$ = $1;
                            }
                            ;
while_stmt                  :   TOK_WHILE { strcpy(compound_stmt_type, "\'while\'"); } test TOK_COLON suite optional_else_suite {
                                    $$ = new node(WHILE_STMT, "WHILE_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "while", true, NULL);
                                    $4 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $1->add_parent_child_relation($3);
                                    $1->add_parent_child_relation($4);
                                    $1->add_parent_child_relation($5);
                                //     $$->add_parent_child_relation($6);
                                    prune_custom_nodes($$, $6);
                            }
                            ;
optional_else_suite         :   TOK_ELSE TOK_COLON suite {
                                    $$ = new node(OPTIONAL_ELSE_SUITE, "OPTIONAL_ELSE_SUITE", false, NULL);
                                    $1 = new node(KEYWORD, "else", true, NULL);
                                    $2 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $1->add_parent_child_relation($2);
                                    $1->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
for_stmt                    :   TOK_FOR { strcpy(compound_stmt_type, "\'for\'"); } exprlist TOK_IN testlist TOK_COLON suite optional_else_suite {
                                    $$ = new node(FOR_STMT, "FOR_STMT", false, NULL);
                                    node* temp = new node(TEST, "TEST", false, NULL);
                                    $1 = new node(KEYWORD, "for", true, NULL);
                                    $4 = new node(KEYWORD, "in", true, NULL);
                                    // $6 = new node(COLON, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $1->add_parent_child_relation(temp);
                                    temp->add_parent_child_relation($3);
                                    temp->add_parent_child_relation($4);
                                    temp->add_parent_child_relation($5);
                                    $1->add_parent_child_relation($6);
                                    $1->add_parent_child_relation($7);
                                //     $$->add_parent_child_relation($8);
                                    prune_custom_nodes($$, $8);
                                }
                            ;

funcdef                     :   TOK_DEF TOK_IDENTIFIER { 
                                    $2 = new node(IDENTIFIER, yytext, true, NULL);
                                    strcpy(compound_stmt_type, "\'function definition\'"); 
                                }
                                parameters optional_tok_rarrow_test TOK_COLON suite {
                                    $$ = new node(FUNCDEF, "FUNCDEF", false, NULL);
                                    $1 = new node(KEYWORD, "def", true, NULL);
                                    $6 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($4);
                                //     $$->add_parent_child_relation($5);
                                    prune_custom_nodes($$, $5);
                                    $$->add_parent_child_relation($6);
                                    $$->add_parent_child_relation($7);
                            }
                            ;
optional_tok_rarrow_test    :   TOK_RARROW test {
                                    $$ = new node(OPTIONAL_TOK_RARROW_TEST, "OPTIONAL_TOK_RARROW_TEST", false, NULL);
                                    $1 = new node(RARROW, "->", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   {   $$ = NULL;  }
                            ;
parameters                  :   TOK_LPAR { join_lines_implicitly++; } optional_typedargslist TOK_RPAR { 
                                    join_lines_implicitly--; 
                                    $$ = new node(PARAMETERS, "PARAMETERS", false, NULL);
                                    $1 = new node(DELIMITER, "(", true, NULL);
                                    $4 = new node(DELIMITER, ")", true, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                                    $$->add_parent_child_relation($4);
                            }
                            ;
optional_typedargslist      :   typedargslist {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
                            ;
typedargslist               :   tfpdef optional_equal_test many_comma_tfpdef_optional_equal_test {
                                    $$ = new node(TYPEDARGSLIST, "TYPEDARGSLIST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $2);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
optional_equal_test         :   TOK_EQUAL test {
                                    $$ = new node(OPTIONAL_EQUAL_TEST, "OPTIONAL_EQUAL_TEST", false, NULL);
                                    $1 = new node(ASSIGN, "=", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   {   $$ = NULL;  }
                            ;
many_comma_tfpdef_optional_equal_test   :   many_comma_tfpdef_optional_equal_test TOK_COMMA tfpdef optional_equal_test {
                                            $$ = new node(MANY_COMMA_TFPDEF_OPTIONAL_EQUAL_TEST, "MANY_COMMA_TFPDEF_OPTIONAL_EQUAL_TEST", false, NULL);
                                            $2 = new node(DELIMITER, ",", true, NULL);
                                        //     $$->add_parent_child_relation($1);
                                            prune_custom_nodes($$, $1);
                                            $$->add_parent_child_relation($2);
                                            $$->add_parent_child_relation($3);
                                        //     $$->add_parent_child_relation($4);
                                            prune_custom_nodes($$, $4);
                                        }
                                        |   {   $$ = NULL;  }
                                        ;
tfpdef                      :   TOK_IDENTIFIER { $1 = new node(IDENTIFIER, yytext, true, NULL); } optional_tok_colon_test {
                                    $$ = new node(TFPDEF, "TFPDEF", false, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
optional_tok_colon_test     :   TOK_COLON test {
                                    $$ = new node(OPTIONAL_TOK_COLON_TEST, "OPTIONAL_TOK_COLON_TEST", false, NULL);
                                    $1 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   {   $$ = NULL;  }
                            ;

classdef                    :   TOK_CLASS TOK_IDENTIFIER { 
                                    $2 = new node(IDENTIFIER, yytext, true, NULL);
                                    strcpy(compound_stmt_type, "\'class definition\'"); 
                                } 
                                optional_paren_arglist TOK_COLON suite {
                                    $$ = new node(CLASSDEF, "CLASSDEF", false, NULL);
                                    $1 = new node(KEYWORD, "class", true, NULL);
                                    $5 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                //     $$->add_parent_child_relation($4);
                                    prune_custom_nodes($$, $4);
                                    $$->add_parent_child_relation($5);
                                    $$->add_parent_child_relation($6);
                            }
                            ;
optional_paren_arglist      :   TOK_LPAR { join_lines_implicitly++; } optional_arglist TOK_RPAR { 
                                    join_lines_implicitly--; 
                                    $$ = new node(OPTIONAL_PAREN_ARGLIST, "OPTIONAL_PAREN_ARGLIST", false, NULL);
                                    $1 = new node(DELIMITER, "(", true, NULL);
                                    $3 = new node(DELIMITER, ")", true, NULL);
                                    $$->add_parent_child_relation($1);
                                //     $$->add_parent_child_relation($3);
                                    prune_custom_nodes($$, $3);
                                    $$->add_parent_child_relation($4);
                            }
                            |   {   $$ = NULL;  }
                            ;
%%
