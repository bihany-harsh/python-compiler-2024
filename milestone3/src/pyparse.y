%{
    #include <iostream>
    #include <stack>
    #include <cstring>
    #include <algorithm>
    #include <set>
    #include "include/node.hpp"
    #include "include/symbol_table.hpp"
    using namespace std;

    extern FILE* yyin;
    extern int yylineno;
    extern bool verbose_flag;
    extern stack<int> INDENT_STACK;

    char compound_stmt_type[64];
    char error_string[256];

    extern node* AST_ROOT;

    extern symbol_table* SYMBOL_TABLE;
    extern stack<symbol_table*> ST_STACK;

    extern stack<int> OFFSET_STACK;
    extern int OFFSET;
    extern int block_counter;
    extern int num_args;

    extern char* yytext;

    int yylex(void);
    void yyerror (char const *s) {
        fprintf (stdout, "line number %d: \t %s\n", yylineno, s);
        exit(-1);
    }

    int join_lines_implicitly = 0;

%}

%union{
    char* str_val;
    struct node* tree_node;
}

%token<tree_node> TOK_IDENTIFIER TOK_INTEGER_NUMBER TOK_FLOAT_NUMBER TOK_IMAG_NUMBER TOK_STRING_LITERAL

%token<tree_node> TOK_NEWLINE TOK_INDENT TOK_DEDENT

%token<tree_node> TOK_FALSE TOK_NONE TOK_TRUE
%token<tree_node> TOK_AND TOK_OR TOK_NOT
%token<tree_node> TOK_BREAK TOK_CONTINUE TOK_RETURN
%token<tree_node> TOK_GLOBAL TOK_NON_LOCAL TOK_DEF TOK_CLASS
%token<tree_node> TOK_IF TOK_ELSE TOK_ELIF
%token<tree_node> TOK_IN TOK_IS
%token<tree_node> TOK_PASS
%token<tree_node> TOK_FOR TOK_WHILE
%token<tree_node> TOK_PRINT TOK_RANGE TOK_SELF TOK_LEN

%token<tree_node> TOK_INT TOK_FLOAT TOK_BOOL TOK_STR TOK_LIST

%token<tree_node> TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_PERCENT TOK_DOUBLE_SLASH TOK_DOUBLE_STAR

%token<tree_node> TOK_EQ_EQUAL TOK_NOT_EQUAL TOK_GREATER TOK_LESS TOK_GREATER_EQUAL TOK_LESS_EQUAL

%token<tree_node> TOK_AMPER TOK_VBAR TOK_CIRCUMFLEX TOK_TILDE TOK_LEFT_SHIFT TOK_RIGHT_SHIFT

%token<tree_node> TOK_EQUAL TOK_PLUS_EQUAL TOK_MINUS_EQUAL TOK_STAR_EQUAL TOK_SLASH_EQUAL TOK_PERCENT_EQUAL
%token<tree_node> TOK_AMPER_EQUAL TOK_VBAR_EQUAL TOK_CIRCUMFLEX_EQUAL TOK_LEFT_SHIFT_EQUAL TOK_RIGHT_SHIFT_EQUAL TOK_DOUBLE_STAR_EQUAL TOK_DOUBLE_SLASH_EQUAL

%token<tree_node> TOK_LPAR TOK_RPAR TOK_LSQB TOK_RSQB TOK_LBRACE TOK_RBRACE
%token<tree_node> TOK_SEMICOLON TOK_COLON TOK_COMMA TOK_DOT
%token<tree_node> TOK_RARROW

%type<tree_node> file_input multiple_lines single_line stmt simple_stmt small_stmt many_small_stmts expr_stmt many_equal_test annassign optional_assign_test
%type<tree_node> augassign testlist many_comma_tok_test optional_comma test optional_if_else or_test many_or_tok_and_test and_test many_and_tok_not_test
%type<tree_node> not_test comparison many_comparison_expr comp_op expr many_vbar_tok_xor_expr xor_expr many_cflex_tok_and_expr and_expr many_amper_tok_shift_expr
%type<tree_node> shift_expr many_shift_op_arith_expr arith_expr many_arith_term term many_mod_factor factor power optional_dstar_tok_factor atom_expr many_trailers
%type<tree_node> atom numeric_strings data_type trailer optional_arglist arglist many_comma_argument argument comp_for optional_comp_iter comp_iter comp_if testlist_comp
%type<tree_node> pass_stmt flow_stmt break_stmt continue_stmt return_stmt optional_test global_stmt many_comma_tok_identifier compound_stmt if_stmt many_elif_stmts
%type<tree_node> optional_else_stmt else_stmt suite at_least_one_stmt while_stmt optional_else_suite for_stmt funcdef optional_tok_rarrow_test parameters
%type<tree_node> optional_typedargslist typedargslist optional_equal_test many_comma_tfpdef_optional_equal_test tfpdef optional_tok_colon_test classdef optional_paren_arglist

%expect 1

%%

file_input                  :   multiple_lines {
                                    $$ = $1;
                                    $$->name = "FILE_INPUT";
                                    $$->type = FILE_INPUT;
                                    $$->st = SYMBOL_TABLE;
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
                                    prune_custom_nodes($$, $3);
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
                            ;
indent_check_small_stmt     :   TOK_INDENT  {   snprintf(error_string, sizeof(error_string), "unexpected indent");
                                                yyerror(error_string);
                                            }
                            |
                            ;
many_small_stmts            :   many_small_stmts TOK_SEMICOLON small_stmt {
                                    $$ = new node(MANY_SMALL_STMTS, "MANY_SMALL_STMTS", false, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
expr_stmt                   :   test annassign {
                                    $$ = new node(EXPR_STMT, "EXPR_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    node* terminal = sem_lval_check($1, 0); // returns nullptr if expecting a class variable declaration
                                    // if invalid, calls yyerror and exits. if valid, returns non-nullptr
                                    base_data_type b_type = sem_rval_check(SYMBOL_TABLE, $2->children[1]);
                                    if(terminal == nullptr) {
                                        // if condition already checked in sem_lval_check() function
                                        // if(SYMBOL_TABLE->st_type != CLASS || (SYMBOL_TABLE->st_type == FUNCTION && SYMBOL_TABLE->st_name != "__init__")) {
                                        //     // self can only be declared in these situations. In all other cases, it is assigned some value
                                        //     yyerror("NameError: name 'self' is not defined");
                                        // }
                                        add_class_st_entry($1, b_type);
                                    }
                                    else {
                                        st_entry* new_entry = new st_entry(terminal->name, b_type, OFFSET, terminal->line_no, SYMBOL_TABLE->scope);
                                        terminal->st = SYMBOL_TABLE;
                                        terminal->st_entry = new_entry;

                                        SYMBOL_TABLE->add_entry(new_entry);
                                        OFFSET += new_entry->size;
                                        if(b_type == D_LIST) {
                                            terminal->set_list_attributes($2);
                                        } else if (b_type == D_CLASS) {
                                            new_entry->class_name = get_class_name($2->children[1]);
                                        } 
                                    }
                                    $$->handle_annassign();
                            }
                            |   test augassign test {
                                    $$ = new node(EXPR_STMT, "EXPR_STMT", false, NULL);
                                    
                                    sem_lval_check($1, 1);

                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    check_declare_before_use(SYMBOL_TABLE, $1); // to check that the LHS has been declared before use
                                    set<string> match = {"+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "**=", "//="};
                                    to_ast_operator($$, false, match);
                            }
                            |   test many_equal_test {
                                    $$ = new node(EXPR_STMT, "EXPR_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    // $$->add_parent_child_relation($2);                              

                                    prune_custom_nodes($$, $2);
                                    sem_lval_check($1, 1);
                                    check_declare_before_use(SYMBOL_TABLE, $1); // to check that the LHS has been declared before use

                                    $$->check_multi_assignment();
                                    $$->update_list_param();
                                    
                                    set<string> match = {"="};
                                    to_ast_operator($$, false, match);
                            }
                            ;
many_equal_test             :   many_equal_test TOK_EQUAL test {
                                    $$ = new node(MANY_EQUAL_TEST, "MANY_EQUAL_TEST", false, NULL);
                                    $2 = new node(ASSIGN, "=", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    // $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    check_declare_before_use(SYMBOL_TABLE, $3); // to check that the RHS has been declared before use
                            }
                            |   many_equal_test TOK_EQUAL {
                                    snprintf(error_string, sizeof(error_string), "SyntaxError: missing rvalue");
                                    yyerror(error_string);
                            }
                            |   {   $$ = NULL;  }
                            ;
annassign                   :   TOK_COLON test optional_assign_test {
                                    $$ = new node(ANNASSIGN, "ANNASSIGN", false, NULL);
                                    $1 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $3);
                                    check_declare_before_use(SYMBOL_TABLE, $2);
                            }
                            ;
optional_assign_test        :   TOK_EQUAL test {
                                    $$ = new node(OPTIONAL_ASSIGN_TEST, "OPTIONAL_ASSIGN_TEST", false, NULL);
                                    $1 = new node(ASSIGN, "=", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    check_declare_before_use(SYMBOL_TABLE, $2);
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
                                    prune_custom_nodes($$, $2);
                                    prune_custom_nodes($$, $3);
                                    check_declare_before_use(SYMBOL_TABLE, $1);
                            }
                            ;
many_comma_tok_test         :   many_comma_tok_test TOK_COMMA test {
                                    $$ = new node(MANY_COMMA_TOK_TEST, "MANY_COMMA_TOK_TEST", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    check_declare_before_use(SYMBOL_TABLE, $3);
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
                                    prune_custom_nodes($$, $2);
                            }
                            ;
optional_if_else            :   TOK_IF or_test TOK_ELSE test {
                                    $$ = new node(OPTIONAL_IF_ELSE, "OPTIONAL_IF_ELSE", false, NULL);
                                    $1 = new node(KEYWORD, "if", true, NULL);
                                    $3 = new node(KEYWORD, "else", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $1->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    $3->add_parent_child_relation($4);
                            }
                            |   {   $$ = NULL;  }
or_test                     :   and_test many_or_tok_and_test {
                                    $$ = new node(OR_TEST, "OR_TEST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"or"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_or_tok_and_test        :   many_or_tok_and_test TOK_OR and_test {
                                    $$ = new node(MANY_OR_TOK_AND_TEST, "MANY_OR_TOK_AND_TEST", false, NULL);
                                    $2 = new node(BOOL_OP, "or", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
and_test                    :   not_test many_and_tok_not_test {
                                    $$ = new node(AND_TEST, "AND_TEST", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"and"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_and_tok_not_test       :   many_and_tok_not_test TOK_AND not_test {
                                    $$ = new node(MANY_AND_TOK_NOT_TEST, "MANY_AND_TOK_NOT_TEST", false, NULL);
                                    $2 = new node(BOOL_OP, "and", true, NULL);
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
                                    prune_custom_nodes($$, $2);
                                    $$->delete_single_child_nodes();
                                    comp_op_processing($$);
                            }
                            ;
many_comparison_expr        :   many_comparison_expr comp_op expr {
                                    $$ = new node(MANY_COMPARISON_EXPR, "MANY_COMPARISON_EXPR", false, NULL);
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
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"|"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_vbar_tok_xor_expr      :   many_vbar_tok_xor_expr TOK_VBAR xor_expr {
                                    $$ = new node(MANY_VBAR_TOK_XOR_EXPR, "MANY_VBAR_TOK_XOR_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "|", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
xor_expr                    :   and_expr many_cflex_tok_and_expr {
                                    $$ = new node(XOR_EXPR, "XOR_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"^"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_cflex_tok_and_expr     :   many_cflex_tok_and_expr TOK_CIRCUMFLEX and_expr {
                                    $$ = new node(MANY_CFLEX_TOK_AND_EXPR, "MANY_CFLEX_TOK_AND_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "^", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
and_expr                    :   shift_expr many_amper_tok_shift_expr {
                                    $$ = new node(AND_EXPR, "AND_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"&"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_amper_tok_shift_expr   :   many_amper_tok_shift_expr TOK_AMPER shift_expr {
                                    $$ = new node(MANY_AMPER_TOK_SHIFT_EXPR, "MANY_AMPER_TOK_SHIFT_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "&", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
shift_expr                  :   arith_expr many_shift_op_arith_expr {
                                    $$ = new node(SHIFT_EXPR, "SHIFT_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"<<", ">>"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_shift_op_arith_expr    :   many_shift_op_arith_expr TOK_LEFT_SHIFT arith_expr {
                                    $$ = new node(MANY_SHIFT_OP_ARITH_EXPR, "MANY_SHIFT_OP_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "<<", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_shift_op_arith_expr TOK_RIGHT_SHIFT arith_expr {
                                    $$ = new node(MANY_SHIFT_OP_ARITH_EXPR, "MANY_SHIFT_OP_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, ">>", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
arith_expr                  :   term many_arith_term {
                                    $$ = new node(ARITH_EXPR, "ARITH_EXPR", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"+", "-"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_arith_term             :   many_arith_term TOK_PLUS term {
                                    $$ = new node(MANY_ARITH_EXPR, "MANY_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "+", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_arith_term TOK_MINUS term {
                                    $$ = new node(MANY_ARITH_EXPR, "MANY_ARITH_EXPR", false, NULL);
                                    $2 = new node(BIN_OP, "-", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
term                        :   factor many_mod_factor {
                                    $$ = new node(TERM, "TERM", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    set<string> match = {"*", "/", "%", "//"};
                                    to_ast_operator($$, true, match);
                            }
                            ;
many_mod_factor             :   many_mod_factor TOK_STAR factor {
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "*", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_mod_factor TOK_SLASH factor {
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "/", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_mod_factor TOK_PERCENT factor {
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "%", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   many_mod_factor TOK_DOUBLE_SLASH factor {
                                    // factor corresponds to sort of using unary operator +, -, ~
                                    $$ = new node(MANY_MOD_FACTOR, "MANY_MOD_FACTOR", false, NULL);
                                    $2 = new node(BIN_OP, "//", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
factor                      :   TOK_PLUS factor {
                                    $$ = new node(UNARY_OP, "+", true, NULL);
                                    $$->add_parent_child_relation($2);
                            }
                            |   TOK_MINUS factor {
                                    $$ = new node(UNARY_OP, "-", true, NULL);
                                    $$->add_parent_child_relation($2);
                            }
                            |   TOK_TILDE factor {
                                    $$ = new node(UNARY_OP, "~", true, NULL);
                                    $$->add_parent_child_relation($2);
                            }
                            |   power {
                                    $$ = $1;
                            }
                            ;
power                       :   atom_expr optional_dstar_tok_factor {
                                    $$ = new node(POWER, "POWER", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
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
atom                        :   TOK_LPAR {join_lines_implicitly++; } test TOK_RPAR {
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
                                    if($$->name == "__name__") {
                                        if(SYMBOL_TABLE->st_type != GLOBAL) {
                                            yyerror("CompilationError: __name__ must be written globally.");
                                        }
                                        SYMBOL_TABLE->get_entry("__name__")->decl_line = yylineno;
                                    }
                            }
                            |   TOK_LEN {
                                    $$ = new node(KEYWORD, "len", true, NULL);
                            }
                            |   TOK_PRINT {
                                    $$ = new node(KEYWORD, "print", true, NULL);
                            }
                            |   TOK_RANGE {
                                    $$ = new node(KEYWORD, "range", true, NULL);
                            }
                            |   TOK_SELF {
                                    $$ = new node(KEYWORD, "self", true, NULL);
                            }
                            |   data_type {
                                    $$ = $1;
                            }
                            |   numeric_strings {
                                    $$ = $1;
                            }
                            |   TOK_STRING_LITERAL {
                                    string str_literal = "\\\"" + string(yytext).substr(1, strlen(yytext) - 2) + "\\\"";
                                    $$ = new node(STRING_LITERAL, str_literal.c_str(), true, NULL);
                            }
                            |   TOK_NONE {
                                    $$ = new node(KEYWORD, "None", true, NULL);
                            }
                            |   TOK_TRUE {
                                    $$ = new node(BOOL_NUMBER, "True", true, NULL);
                            }
                            |   TOK_FALSE {
                                    $$ = new node(BOOL_NUMBER, "False", true, NULL);
                            }
                            ;
numeric_strings             :   TOK_INTEGER_NUMBER {
                                    $$ = new node(INT_NUMBER, yytext, true, NULL);
                            }
                            |   TOK_FLOAT_NUMBER {
                                    $$ = new node(FLOAT_NUMBER, yytext, true, NULL);
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
                            |   TOK_LIST {
                                    $$ = new node(LIST, "list", true, NULL);
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
                                    prune_custom_nodes($$, $3); 
                                    $$->add_parent_child_relation($4);
                            }
                            |   TOK_LSQB { join_lines_implicitly++; } test TOK_RSQB {
                                    join_lines_implicitly--;
                                    $$ = new node(TRAILER, "TRAILER", false, NULL);
                                    $1 = new node(DELIMITER, "[", true, NULL);
                                    $4 = new node(DELIMITER, "]", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($3);
                                    $$->add_parent_child_relation($4);
                            }
                            |   TOK_DOT TOK_IDENTIFIER {
                                    $$ = new node(TRAILER, "TRAILER", false, NULL);
                                    $1 = new node(DOT, ".", true, NULL);
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
                                    prune_custom_nodes($$, $2);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
many_comma_argument         :   many_comma_argument TOK_COMMA argument {
                                    $$ = new node(MANY_COMMA_ARGUMENT, "MANY_COMMA_ARGUMENT", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL;  }
                            ;
argument                    :   test {
                                $$ = $1;
                            }
                            |   test TOK_EQUAL test {
                                    $$ = new node(ARGUMENT, "ARGUMENT", false, NULL);
                                    $2 = new node(ASSIGN, "=", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            ;
comp_for                    :   TOK_FOR expr TOK_IN or_test optional_comp_iter {
                                    $$ = new node(COMP_FOR, "COMP_FOR", false, NULL);
                                    $1 = new node(KEYWORD, "for", true, NULL);
                                    $3 = new node(KEYWORD, "in", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                                    $$->add_parent_child_relation($4);
                                    prune_custom_nodes($$, $5);
                            }
                            ;
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
comp_if                     :   TOK_IF or_test optional_comp_iter {
                                    $$ = new node(COMP_IF, "COMP_IF", false, NULL);
                                    $1 = new node(KEYWORD, "if", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
testlist_comp               :   test comp_for {
                                    $$ = new node(TESTLIST_COMP, "TESTLIST_COMP", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    check_declare_before_use(SYMBOL_TABLE, $1);
                            }
                            |   test many_comma_tok_test optional_comma {
                                    $$ = new node(TESTLIST_COMP, "TESTLIST_COMP", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    prune_custom_nodes($$, $3);
                                    check_declare_before_use(SYMBOL_TABLE, $1);
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
return_stmt                 :   TOK_RETURN optional_test {
                                    $$ = new node(RETURN_STMT, "RETURN_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "return", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $2);
                                    if(SYMBOL_TABLE->st_type != FUNCTION) {
                                        yyerror("SyntaxError: 'return' outside function");
                                    }
                            }
                            |   TOK_RETURN test TOK_COMMA testlist {
                                    yyerror("SyntaxError: Attempting to return multiple values from a function");
                            }
                            ;
optional_test               :   test {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
                            ;
global_stmt                 :   TOK_GLOBAL TOK_IDENTIFIER { 
                                    $2 = new node(IDENTIFIER, yytext, true, NULL); 
                                    } many_comma_tok_identifier {
                                    $$ = new node(GLOBAL_STMT, "GLOBAL_STMT", false, NULL);
                                    $1 = new node(KEYWORD, "global", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($4);
                                    yyerror("VersionError: Glboal statements not supported.");
                            }
                            ;
many_comma_tok_identifier   :   many_comma_tok_identifier TOK_COMMA TOK_IDENTIFIER {
                                    $$ = new node(MANY_COMMA_TOK_IDENTIFIER, "MANY_COMMA_TOK_IDENTIFIER", false, NULL);
                                    $2 = new node(DELIMITER, ",", true, NULL);
                                    $3 = new node(IDENTIFIER, yytext, true, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($3);
                            }
                            |   {   $$ = NULL; }
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

if_stmt                     :   TOK_IF { strcpy(compound_stmt_type, "\'if\'"); } test TOK_COLON {
                                        // change the scope. This is a new block
                                        $1 = new node(KEYWORD, "if", true, NULL);
                                        $4 = new node(DELIMITER, ":", true, NULL);

                                        node* tmp = new node(CONDITION, "CONDITION", false, NULL);
                                        $1->add_parent_child_relation(tmp);
                                        tmp->add_parent_child_relation($3);
                                        
                                        $1->add_parent_child_relation($4);
                                        check_declare_before_use(SYMBOL_TABLE, $3);
                                        // change the scope. This is a new block
                                        // $1->setup_new_st_env();
                                    } suite {
                                        // change the scope back to the parent block
                                        // $1->create_block_st("IF_BLOCK");
                                        $1->add_parent_child_relation($6);
                                    }
                                    many_elif_stmts optional_else_stmt {
                                    $$ = new node(IF_STMT, "IF_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $8);
                                    prune_custom_nodes($$, $9);
                            }
                            |   TOK_IF TOK_COLON {
                                    snprintf(error_string, sizeof(error_string), "SyntaxError: invalid syntax");
                                    yyerror(error_string);
                            }
                            ;
many_elif_stmts             :   many_elif_stmts TOK_ELIF { strcpy(compound_stmt_type, "\'elif\'"); } test TOK_COLON {
                                    $2 = new node(KEYWORD, "elif", true, NULL);
                                    $5 = new node(DELIMITER, ":", true, NULL);

                                    // for 3AC generation of elif
                                    node* tmp = new node(CONDITION, "CONDITION", false, NULL);
                                    $2->add_parent_child_relation(tmp);
                                    tmp->add_parent_child_relation($4);

                                    $2->add_parent_child_relation($5);
                                    check_declare_before_use(SYMBOL_TABLE, $4);
                                    // change the scope. This is a new block
                                //     $2->setup_new_st_env();
                                } suite {
                                    // change the scope back to the parent block
                                //     $2->create_block_st("ELIF_BLOCK");

                                    $$ = new node(MANY_ELIF_STMTS, "MANY_ELIF_STMTS", false, NULL);
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                                    $2->add_parent_child_relation($7);
                            }
                            |   {   $$ = NULL;  }
optional_else_stmt          :   else_stmt {
                                    $$ = $1;
                            }
                            |   {   $$ = NULL;  }
else_stmt                   :   TOK_ELSE { strcpy(compound_stmt_type, "\'else\'"); } TOK_COLON {
                                        $1 = new node(KEYWORD, "else", true, NULL);
                                        $3 = new node(DELIMITER, ":", true, NULL);
                                        $1->add_parent_child_relation($3);

                                        // create a new scope
                                        // $1->setup_new_st_env();
                                    }
                                    suite {
                                        // restore the scope
                                        // $1->create_block_st("ELSE_BLOCK");

                                        $$ = new node(ELSE_STMT, "ELSE_STMT", false, NULL);
                                        $$->add_parent_child_relation($1);
                                        $1->add_parent_child_relation($5);
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
                                    prune_custom_nodes($$, $1);
                                    $$->add_parent_child_relation($2);
                            }
                            |   stmt {
                                    $$ = new node(AT_LEAST_ONE_STMT, "AT_LEAST_ONE_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                            }
                            ;
while_stmt                  :   TOK_WHILE { strcpy(compound_stmt_type, "\'while\'"); } test TOK_COLON {
                                        check_declare_before_use(SYMBOL_TABLE, $3);
                                        $1 = new node(KEYWORD, "while", true, NULL);
                                        $4 = new node(DELIMITER, ":", true, NULL);

                                        node* tmp = new node(CONDITION, "CONDITION", false, NULL);
                                        $1->add_parent_child_relation(tmp);
                                        tmp->add_parent_child_relation($3);

                                        $1->add_parent_child_relation($4);
                                        // $1->setup_new_st_env();
                                    } suite {
                                        // change the scope back to the parent block
                                        // $1->create_block_st("WHILE_BLOCK");
                                        $1->add_parent_child_relation($6);
                                    } optional_else_suite {
                                    $$ = new node(WHILE_STMT, "WHILE_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($8);
                            }
                            ;
optional_else_suite         :   TOK_ELSE TOK_COLON {
                                    $1 = new node(KEYWORD, "else", true, NULL);
                                    $2 = new node(DELIMITER, ":", true, NULL);
                                    $1->add_parent_child_relation($2);
                                    
                                    // change the scope. This is a new block
                                //     $1->setup_new_st_env();
                                } suite {
                                    // change the scope back to the parent block
                                //     $1->create_block_st("ELSE_BLOCK");
                                    $1->add_parent_child_relation($4);
                            }
                            |   {   $$ = NULL;  }
                            ;
for_stmt                    :   TOK_FOR { strcpy(compound_stmt_type, "\'for\'"); } expr TOK_IN test TOK_COLON {
                                    $1 = new node(KEYWORD, "for", true, NULL);
                                    node* temp = new node(CONDITION, "CONDITION", false, NULL);
                                    $4 = new node(KEYWORD, "in", true, NULL);
                                    
                                    $1->add_parent_child_relation(temp);
                                    temp->add_parent_child_relation($3);
                                    temp->add_parent_child_relation($4);
                                    temp->add_parent_child_relation($5);
                                //     $1->setup_new_st_env();
                                } suite {
                                    // change the scope back to the parent block
                                //     $1->create_block_st("FOR_BLOCK");
                                    $1->add_parent_child_relation($8);
                                } optional_else_suite {
                                    $$ = new node(FOR_STMT, "FOR_STMT", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $10);
                                }
                            ;

funcdef                     :   TOK_DEF TOK_IDENTIFIER {
                                    $1 = new node(KEYWORD, "def", true, NULL);
                                    $2 = new node(IDENTIFIER, yytext, true, NULL);
                                    if(SYMBOL_TABLE->st_type == FUNCTION) {
                                        // trying to declare function inside another function/block
                                        yyerror("SyntaxError: Nested functions are not supported");
                                    }
                                    // not considering block as another scope
                                //     else if(SYMBOL_TABLE->st_type == BLOCK) {
                                //         yyerror("SyntaxError: Improper function declaration.");
                                //     }
                                    $2->create_func_st();
                                    strcpy(compound_stmt_type, "\'function definition\'");
                                }
                                parameters {
                                    $2->st_entry->set_num_args(num_args);
                                    num_args = 0;
                                } optional_tok_rarrow_test {
                                    if($6 == NULL) {
                                        // currently in function environment, we have to look at the parent environment
                                        if((SYMBOL_TABLE->parent->st_type == GLOBAL && $2->name != "main") || (SYMBOL_TABLE->parent->st_type == CLASS && $2->name != "__init__")) {
                                            yyerror("Function must have a return type");
                                        }
                                        else {
                                            // `main` or `__init__` function
                                            $6 = new node(OPTIONAL_TOK_RARROW_TEST, "OPTIONAL_TOK_RARROW_TEST", false, NULL);
                                            node* tmp1 = new node(RARROW, "->", true, NULL);
                                            node* tmp2 = new node(KEYWORD, "None", true, NULL);
                                            $6->add_parent_child_relation(tmp1);
                                            $6->add_parent_child_relation(tmp2);
                                        }
                                        // else we would want to set the return type to void : set by default
                                    }
                                    else {
                                        $2->st_entry->set_return_type(sem_rval_check(SYMBOL_TABLE, $6->children[1]));
                                        if($2->st_entry->f_attr.return_type == D_LIST) {
                                            set_return_list_attr($2->st_entry, get_base_type_of_list($6->children[1]));
                                        }
                                    }
                                    $2->set_function_attr_and_return();
                                } TOK_COLON suite {
                                    $$ = new node(FUNCDEF, "FUNCDEF", false, NULL);
                                    $8 = new node(DELIMITER, ":", true, NULL);

                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($4);
                                    prune_custom_nodes($$, $6);
                                    $$->add_parent_child_relation($8);
                                    $$->add_parent_child_relation($9);

                                    $$->delete_delimiters();
                                    $$->delete_single_child_nodes();
                                    string label = make_function_label($$);
                                    $2->exit_from_func(); // sets the argument types of the function and restores scope
                                    $2->st_entry->label = label;
                            }
                            ;
optional_tok_rarrow_test    :   TOK_RARROW test {
                                    $$ = new node(OPTIONAL_TOK_RARROW_TEST, "OPTIONAL_TOK_RARROW_TEST", false, NULL);
                                    $1 = new node(RARROW, "->", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    check_declare_before_use(SYMBOL_TABLE, $2);
                            }
                            |   {   $$ = NULL;  }
                            ;
parameters                  :   TOK_LPAR  {join_lines_implicitly++;} optional_typedargslist TOK_RPAR {
                                    join_lines_implicitly--;
                                    $$ = new node(PARAMETERS, "PARAMETERS", false, NULL);
                                    prune_custom_nodes($$, $3);
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
                                    prune_custom_nodes($$, $2);
                                    prune_custom_nodes($$, $3);
                            }
                            ;
optional_equal_test         :   TOK_EQUAL test {
                                    $$ = new node(OPTIONAL_EQUAL_TEST, "OPTIONAL_EQUAL_TEST", false, NULL);
                                    $1 = new node(ASSIGN, "=", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    check_declare_before_use(SYMBOL_TABLE, $2); // if assigning a value to function argument, must be defined beforehand
                            }
                            |   {   $$ = NULL;  }
                            ;
many_comma_tfpdef_optional_equal_test   :   many_comma_tfpdef_optional_equal_test TOK_COMMA tfpdef optional_equal_test {
                                            $$ = new node(MANY_COMMA_TFPDEF_OPTIONAL_EQUAL_TEST, "MANY_COMMA_TFPDEF_OPTIONAL_EQUAL_TEST", false, NULL);
                                            prune_custom_nodes($$, $1);
                                            $$->add_parent_child_relation($3);
                                            prune_custom_nodes($$, $4);
                                        }
                                        |   {   $$ = NULL;  }
                                        ;
tfpdef                      :   TOK_IDENTIFIER { $1 = new node(IDENTIFIER, yytext, true, NULL); } optional_tok_colon_test {
                                    // for now, we can keep it as optional_tok_colon_test since ```self``` will not have a type specification
                                    if($3 != NULL) {
                                        num_args++;
                                        base_data_type b_type = sem_rval_check(SYMBOL_TABLE, $3->children[1]); // passing the node of ```test``` to rval_check
                                        st_entry* new_entry = new st_entry($1->name, b_type, OFFSET, $1->line_no, SYMBOL_TABLE->scope);

                                        if (b_type == D_LIST) {
                                            set_list_elem_type(SYMBOL_TABLE, $3->children[1], new_entry);
                                        }

                                        OFFSET += new_entry->size;
                                        SYMBOL_TABLE->add_entry(new_entry);
                                        $1->st = SYMBOL_TABLE;
                                        $1->st_entry = new_entry;
                                    }
                                    $$ = new node(TFPDEF, "TFPDEF", false, NULL);
                                    $$->add_parent_child_relation($1);
                                    if($3 == NULL) {
                                        yyerror(("SyntaxError: missing type specification for function argument " + $1->name).c_str());
                                    }
                                    prune_custom_nodes($$, $3);
                            }
                            |   TOK_SELF {
                                // num_args++;
                                if (SYMBOL_TABLE->parent->st_type != CLASS) {
                                    yyerror("SyntaxError: `self` not defined.");
                                }
                                $$ = new node(KEYWORD, "self", true, NULL);
                            }
                            ;
optional_tok_colon_test     :   TOK_COLON test {
                                    $$ = new node(OPTIONAL_TOK_COLON_TEST, "OPTIONAL_TOK_COLON_TEST", false, NULL);
                                    $1 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    check_declare_before_use(SYMBOL_TABLE, $2);
                            }
                            |   { $$ = NULL; }
                            ;

classdef                    :   TOK_CLASS TOK_IDENTIFIER {
                                    $2 = new node(IDENTIFIER, yytext, true, NULL);
                                    strcpy(compound_stmt_type, "\'class definition\'");
                                    if(SYMBOL_TABLE->st_type != GLOBAL) {
                                        yyerror("SyntaxError: class declared inside another block");
                                    }
                                    $2->create_class_st();
                                }
                                optional_paren_arglist TOK_COLON suite {
                                    $$ = new node(CLASSDEF, "CLASSDEF", false, NULL);
                                    $1 = new node(KEYWORD, "class", true, NULL);
                                    $5 = new node(DELIMITER, ":", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    $$->add_parent_child_relation($2);
                                    $$->add_parent_child_relation($4);
                                    // prune_custom_nodes($$, $4);
                                    $$->add_parent_child_relation($5);
                                    $$->add_parent_child_relation($6);

                                    $2->handle_inheritance($4);
                                    $2->exit_from_class();
                            }
                            ;
optional_paren_arglist      :   TOK_LPAR { join_lines_implicitly++; } optional_arglist TOK_RPAR { 
                                    join_lines_implicitly--; 
                                    $$ = new node(OPTIONAL_PAREN_ARGLIST, "OPTIONAL_PAREN_ARGLIST", false, NULL);
                                    $1 = new node(DELIMITER, "(", true, NULL);
                                    $4 = new node(DELIMITER, ")", true, NULL);
                                    $$->add_parent_child_relation($1);
                                    prune_custom_nodes($$, $3);
                                    $$->add_parent_child_relation($4);
                            }
                            |   {   $$ = NULL;  }
                            ;
%%
