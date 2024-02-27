%{
    #include <iostream>
    #include <stack>
    #include <cstring>
    using namespace std;

    extern FILE* yyin;
    extern int yylineno;
    extern stack<int> INDENT_STACK;

    char compound_stmt_type[64];
    char error_string[256];

    // bool line_flag;
    extern char* yytext;

    int yylex(void);
    void yyerror (char const *s) {
        fprintf (stderr, "line number %d: \t %s\n", yylineno, s);
        exit(-1);
    }

    int join_lines_implicitly = 0;

    // TODO: funcdef ")" error check
%}

%token TOK_IDENTIFIER TOK_NUMBER TOK_STRING_LITERAL

%token TOK_NEWLINE TOK_INDENT TOK_DEDENT

%token TOK_FALSE TOK_NONE TOK_TRUE
%token TOK_AND TOK_OR TOK_NOT
%token TOK_BREAK TOK_CONTINUE TOK_RETURN
%token TOK_GLOBAL TOK_NON_LOCAL TOK_DEF TOK_CLASS
%token TOK_IF TOK_ELSE TOK_ELIF
%token TOK_IN TOK_IS
%token TOK_PASS
%token TOK_FOR TOK_WHILE

%token TOK_INT TOK_FLOAT TOK_BOOL TOK_STR

%token TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_PERCENT TOK_DOUBLE_SLASH TOK_DOUBLE_STAR

%token TOK_EQ_EQUAL TOK_NOT_EQUAL TOK_GREATER TOK_LESS TOK_GREATER_EQUAL TOK_LESS_EQUAL

%token TOK_AMPER TOK_VBAR TOK_CIRCUMFLEX TOK_TILDE TOK_LEFT_SHIFT TOK_RIGHT_SHIFT

%token TOK_EQUAL TOK_PLUS_EQUAL TOK_MINUS_EQUAL TOK_STAR_EQUAL TOK_SLASH_EQUAL TOK_PERCENT_EQUAL 
%token TOK_AMPER_EQUAL TOK_VBAR_EQUAL TOK_CIRCUMFLEX_EQUAL TOK_LEFT_SHIFT_EQUAL TOK_RIGHT_SHIFT_EQUAL TOK_DOUBLE_STAR_EQUAL TOK_DOUBLE_SLASH_EQUAL

%token TOK_LPAR TOK_RPAR TOK_LSQB TOK_RSQB TOK_LBRACE TOK_RBRACE
%token TOK_SEMICOLON TOK_COLON TOK_COMMA TOK_DOT
%token TOK_RARROW

%expect 1

%%

file_input                  :   multiple_lines 
                            ;
multiple_lines              :   multiple_lines single_line
                            |
                            ;
single_line                 :   stmt
                            |   TOK_NEWLINE
                            ;
stmt                        :   simple_stmt 
                            |   compound_stmt 
                            ;
simple_stmt                 :   indent_check_small small_stmt many_small_stmts optional_semicolon TOK_NEWLINE
                            ;
optional_semicolon          :   TOK_SEMICOLON
                            |
                            ;
small_stmt                  :   expr_stmt
                            |   pass_stmt
                            |   flow_stmt
                            |   global_stmt
                            |   nonlocal_stmt
                             /* TODO: checking for requirement of assert stmt only */
                            ;
indent_check_small          :   TOK_INDENT  {   snprintf(error_string, sizeof(error_string), "unexpected indent");
                                                yyerror(error_string);
                                            }
                            |
                            ;
many_small_stmts            :   many_small_stmts TOK_SEMICOLON small_stmt
                            |
                            ;
expr_stmt                   :   testlist_star_expr annassign
                            |   testlist_star_expr augassign testlist
                            |   testlist_star_expr many_equal_testlist_star_expr
                            ;
testlist_star_expr          :   test
                            // |   star_expr
                            ;
many_equal_testlist_star_expr   :   many_equal_testlist_star_expr TOK_EQUAL testlist_star_expr
                            |
                            ;
annassign                   :   TOK_COLON test optional_assign_test
                            ;
optional_assign_test        :   TOK_EQUAL test
                            |
                            ;
augassign                   :   TOK_PLUS_EQUAL
                            |   TOK_MINUS_EQUAL
                            |   TOK_STAR_EQUAL
                            |   TOK_SLASH_EQUAL
                            |   TOK_PERCENT_EQUAL
                            |   TOK_AMPER_EQUAL
                            |   TOK_VBAR_EQUAL
                            |   TOK_CIRCUMFLEX_EQUAL
                            |   TOK_LEFT_SHIFT_EQUAL
                            |   TOK_RIGHT_SHIFT_EQUAL
                            |   TOK_DOUBLE_STAR_EQUAL
                            |   TOK_DOUBLE_SLASH_EQUAL
                            ;
testlist                    :   test many_comma_tok_test optional_comma
                            ;
many_comma_tok_test         :   many_comma_tok_test TOK_COMMA test
                            |
                            ;
optional_comma              :   TOK_COMMA
                            |
                            ;
                            
test                        :   or_test optional_if_else
                            ;
optional_if_else            :   TOK_IF or_test TOK_ELSE test
                            |
                            ;
or_test                     :   and_test many_or_tok_and_test
                            ;
many_or_tok_and_test        :   many_or_tok_and_test TOK_OR and_test
                            |
                            ;
and_test                    :   not_test many_and_tok_not_test
                            ;
many_and_tok_not_test       :   many_and_tok_not_test TOK_AND not_test
                            |
                            ;
not_test                    :   TOK_NOT not_test
                            |   comparison
                            ;
comparison                  :   expr many_comparison_expr
                            ;
many_comparison_expr        :   many_comparison_expr comp_op expr
                            |
                            ;
comp_op                     :   TOK_LESS
                            |   TOK_GREATER
                            |   TOK_EQ_EQUAL
                            |   TOK_GREATER_EQUAL
                            |   TOK_LESS_EQUAL
                            |   TOK_NOT_EQUAL
                            |   TOK_IN
                            |   TOK_NOT TOK_IN
                            |   TOK_IS
                            |   TOK_IS TOK_NOT
                            ;
expr                        :   xor_expr many_vbar_tok_xor_expr
                            ;
many_vbar_tok_xor_expr      :   many_vbar_tok_xor_expr TOK_VBAR xor_expr
                            |
                            ;
xor_expr                    :   and_expr many_cflex_tok_and_expr
                            ;
many_cflex_tok_and_expr     :   many_cflex_tok_and_expr TOK_CIRCUMFLEX and_expr
                            |
                            ;
and_expr                    :   shift_expr many_amper_tok_shift_expr
                            ;
many_amper_tok_shift_expr   :   many_amper_tok_shift_expr TOK_AMPER shift_expr
                            |
                            ;
shift_expr                  :   arith_expr many_shift_op_arith_expr
                            ;
many_shift_op_arith_expr    :   many_shift_op_arith_expr TOK_LEFT_SHIFT arith_expr
                            |   many_shift_op_arith_expr TOK_RIGHT_SHIFT arith_expr
                            |
                            ;   
arith_expr                  :   term many_arith_term
                            ;
many_arith_term             :   many_arith_term TOK_PLUS term
                            |   many_arith_term TOK_MINUS term
                            |
                            ;
term                        :   factor many_mod_factor
                            ;
many_mod_factor             :   many_mod_factor TOK_STAR factor
                            |   many_mod_factor TOK_SLASH factor
                            |   many_mod_factor TOK_PERCENT factor
                            |   many_mod_factor TOK_DOUBLE_SLASH factor
                            |
                            ;
factor                      :   TOK_PLUS factor
                            |   TOK_MINUS factor
                            |   TOK_TILDE factor
                            |   power
                            ;
power                       :   atom_expr optional_dstar_tok_factor
                            ;
optional_dstar_tok_factor   :   TOK_DOUBLE_STAR factor
                            |
                            ;
atom_expr                   :   atom many_trailers
                            ;
atom                        :   TOK_LPAR { join_lines_implicitly = 1; } testlist_comp TOK_RPAR { join_lines_implicitly = 0; }
                            |   TOK_LSQB { join_lines_implicitly = 1; } testlist_comp TOK_RSQB { join_lines_implicitly = 0; }
                            |   TOK_IDENTIFIER //TODO: identifier may nnot be required here
                            |   data_type
                            |   TOK_NUMBER
                            |   at_least_one_string
                            |   TOK_NONE
                            |   TOK_TRUE
                            |   TOK_FALSE
                            ;
data_type                   :   TOK_INT
                            |   TOK_FLOAT
                            |   TOK_STR
                            |   TOK_BOOL
                            ;
at_least_one_string         :   at_least_one_string TOK_STRING_LITERAL
                            |   TOK_STRING_LITERAL
                            ;
many_trailers               :   many_trailers trailer
                            |
                            ;
trailer                     :   TOK_LPAR { join_lines_implicitly = 1; } optional_arglist TOK_RPAR { join_lines_implicitly = 0; }
                            |   TOK_LSQB { join_lines_implicitly = 1; } subscriptlist TOK_RSQB { join_lines_implicitly = 0; }
                            |   TOK_DOT TOK_IDENTIFIER
                            ;
optional_arglist            :   arglist
                            |
                            ;
arglist                     :   argument many_comma_argument optional_comma 
                            ;
many_comma_argument         :   many_comma_argument TOK_COMMA argument
                            |
                            ;
argument                    :   test optional_comp_for
                            |   test TOK_EQUAL test
                            ;
subscriptlist               :   subscript many_comma_subscript optional_comma
                            ;
subscript                   :   test
                            ;
many_comma_subscript        :   many_comma_subscript TOK_COMMA subscript
                            |  
                            ;
optional_comp_for           :   comp_for
                            |   
                            ;
comp_for                    :   TOK_FOR exprlist TOK_IN or_test optional_comp_iter
                            ;
exprlist                    :   expr many_comma_expr optional_comma
                            ;
many_comma_expr             :   many_comma_expr TOK_COMMA expr
                            |
                            ;
optional_comp_iter          :   comp_iter
                            |
                            ;
comp_iter                   :   comp_for
                            |   comp_if
                            ;
comp_if                     :   TOK_IF test_nocond optional_comp_iter
                            ;
test_nocond                 :   or_test
                            ;
testlist_comp               :   test comp_for
                            |   test many_comma_tok_test optional_comma
                            ;

pass_stmt                   :   TOK_PASS
                            ;

flow_stmt                   :   break_stmt
                            |   continue_stmt
                            |   return_stmt
                            //NOTE: not supporting raise statement
                            ;
break_stmt                  :   TOK_BREAK
                            ;
continue_stmt               :   TOK_CONTINUE
                            ;
return_stmt                 :   TOK_RETURN optional_testlist
                            ;
optional_testlist           :   testlist
                            |
                            ;

global_stmt                 :   TOK_GLOBAL TOK_IDENTIFIER many_comma_tok_identifier
                            ;
many_comma_tok_identifier   :   many_comma_tok_identifier TOK_COMMA TOK_IDENTIFIER
                            |
                            ;

nonlocal_stmt               :   TOK_NON_LOCAL TOK_IDENTIFIER many_comma_tok_identifier
                            ;

compound_stmt               :   if_stmt
                            |   while_stmt
                            |   for_stmt
                            |   funcdef
                            |   classdef
                            ;

if_stmt                     :   TOK_IF { strcpy(compound_stmt_type, "\'if\'"); } test TOK_COLON suite many_elif_stmts optional_else_stmt
                            ;
many_elif_stmts             :   many_elif_stmts TOK_ELIF { strcpy(compound_stmt_type, "\'elif\'"); } test TOK_COLON suite
                            |
                            ;
optional_else_stmt          :   else_stmt
                            |
                            ;
else_stmt                   :   TOK_ELSE { strcpy(compound_stmt_type, "\'else\'"); } TOK_COLON suite
                            ;
suite                       :   simple_stmt
                            |   TOK_NEWLINE indent_check_compound at_least_one_stmt TOK_DEDENT
                            ;
indent_check_compound       :   TOK_INDENT
                            |   {
                                    snprintf(error_string, sizeof(error_string), "IndentationFault: expected an indented block after %s", compound_stmt_type); 
                                    yyerror(error_string); 
                                }   
                            ;
at_least_one_stmt           :   at_least_one_stmt stmt
                            |   stmt
                            ;

while_stmt                  :   TOK_WHILE { strcpy(compound_stmt_type, "\'while\'"); } test TOK_COLON suite optional_else_suite
                            ;
optional_else_suite         :   TOK_ELSE TOK_COLON suite
                            |   
                            ;
for_stmt                    :   TOK_FOR { strcpy(compound_stmt_type, "\'for\'"); } exprlist TOK_IN testlist TOK_COLON suite optional_else_suite
                            ;

funcdef                     :   TOK_DEF TOK_IDENTIFIER { strcpy(compound_stmt_type, "\'function definition\'"); } parameters optional_tok_rarrow_test TOK_COLON suite
                            ;
optional_tok_rarrow_test    :   TOK_RARROW test
                            |
                            ;
parameters                  :   TOK_LPAR { join_lines_implicitly = 1; } optional_typedargslist TOK_RPAR { join_lines_implicitly = 0; }
                            ;
optional_typedargslist      :   typedargslist
                            |
                            ;
typedargslist               :   tfpdef optional_equal_test many_comma_tfpdef_optional_equal_test
                            ;
optional_equal_test         :   TOK_EQUAL test
                            |
                            ;
many_comma_tfpdef_optional_equal_test   :   many_comma_tfpdef_optional_equal_test TOK_COMMA tfpdef optional_equal_test
                                        |   
                                        ;
tfpdef                      :   TOK_IDENTIFIER optional_tok_colon_test
                            ;
optional_tok_colon_test     :   TOK_COLON test
                            |
                            ;

classdef                    :   TOK_CLASS TOK_IDENTIFIER { strcpy(compound_stmt_type, "\'class definition\'"); } optional_paren_arglist TOK_COLON suite
                            ;
optional_paren_arglist      :   TOK_LPAR { join_lines_implicitly = 1; } optional_arglist TOK_RPAR { join_lines_implicitly = 0; }
                            |   
                            ;
%%

int main(int argc, const char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }
    INDENT_STACK.push(0);
    yyparse();

    return 0;
}