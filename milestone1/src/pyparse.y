%{
    #include <iostream>
    using namespace std;

    extern FILE* yyin;
    extern int yylineno;

    int yylex(void);
    void yyerror (char const *s) {
        fprintf (stderr, "%s\n", s);
    }
%}

%token TOK_IDENTIFIER TOK_NUMBER TOK_STRING_LITERAL
%token TOK_GLOBAL TOK_NON_LOCAL

%token TOK_NEWLINE TOK_INDENT TOK_DEDENT

%token TOK_FALSE TOK_NONE TOK_TRUE
%token TOK_AND TOK_OR TOK_NOT
%token TOK_PASS
%token TOK_BREAK TOK_CONTINUE TOK_RETURN
%token TOK_IF TOK_ELSE TOK_ELIF
%token TOK_IN TOK_IS
%token TOK_FOR TOK_WHILE

%token TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_PERCENT TOK_DOUBLE_SLASH TOK_DOUBLE_STAR
%token TOK_EQ_EQUAL TOK_NOT_EQUAL TOK_GREATER TOK_LESS TOK_GREATER_EQUAL TOK_LESS_EQUAL
%token TOK_AMPER TOK_VBAR TOK_CIRCUMFLEX TOK_TILDE TOK_LEFT_SHIFT TOK_RIGHT_SHIFT
%token TOK_EQUAL TOK_PLUS_EQUAL TOK_MINUS_EQUAL TOK_STAR_EQUAL TOK_SLASH_EQUAL TOK_PERCENT_EQUAL 
%token TOK_AMPER_EQUAL TOK_VBAR_EQUAL TOK_CIRCUMFLEX_EQUAL TOK_LEFT_SHIFT_EQUAL TOK_RIGHT_SHIFT_EQUAL TOK_DOUBLE_STAR_EQUAL TOK_DOUBLE_SLASH_EQUAL

%token TOK_LPAR TOK_RPAR TOK_LSQB TOK_RSQB TOK_LBRACE TOK_RBRACE
%token TOK_SEMICOLON TOK_COLON TOK_COMMA TOK_DOT

%%

/* handle TOK_NEWLINE properly. Ensure every single line ends in a TOK_NEWLINE TODO: tbd in perhaps compound stmt*/

file_input                  :   multiple_lines 
                            ;
multiple_lines              :   multiple_lines single_line
                            |
                            ;
single_line                 :   simple_stmt
                            |   compound_stmt TOK_NEWLINE /* why is it needed here? tbchecked */
                            |   TOK_NEWLINE
                            ;
simple_stmt                 :   small_stmt many_small_stmts optional_semicolon TOK_NEWLINE
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
many_equal_testlist_star_expr     :   many_equal_testlist_star_expr TOK_EQUAL testlist_star_expr
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
                            //TODO: If await is supported, to be added here
atom_expr                   :   atom many_trailers
                            ;
atom                        :   TOK_LPAR testlist_comp TOK_RPAR
                            |   TOK_LSQB testlist_comp TOK_RSQB
                            |   TOK_IDENTIFIER
                            |   TOK_NUMBER
                            |   at_least_one_string
                            |   TOK_NONE
                            |   TOK_TRUE
                            |   TOK_FALSE
                            ;
at_least_one_string         :   at_least_one_string TOK_STRING_LITERAL
                            |   TOK_STRING_LITERAL
                            ;
many_trailers               :   many_trailers trailer
                            |
                            ;
trailer                     :   TOK_LPAR optional_arglist TOK_RPAR
                            |   TOK_LSQB subscriptlist TOK_RSQB
                            |   TOK_DOT TOK_IDENTIFIER
                            ;
optional_arglist            :   arglist
                            |
                            ;
arglist                     :   argument many_comma_argument optional_comma //optional_comma defined previously
                            ;
many_comma_argument         :   many_comma_argument TOK_COMMA argument
                            |
                            ;
argument                    :   test optional_comp_for
                            |   test TOK_EQUAL test
                            //     /* most probably redudant as represents *args, **kwags*/
                            // |   TOK_DOUBLE_STAR test
                            // |   TOK_STAR test
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
                            /*TODO: to support or not: ASYNC*/
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
                            //TODO: many others to be added
                            ;

if_stmt                     :   TOK_IF test TOK_COLON suite many_elif_stmts optional_else_stmt
                            ;
many_elif_stmts             :   many_elif_stmts TOK_ELIF test TOK_COLON suite
                            |
                            ;
optional_else_stmt          :   else_stmt
                            |
                            ;
else_stmt                   :   TOK_ELSE TOK_COLON suite
                            ;
suite                       :   simple_stmt
                            |   TOK_NEWLINE TOK_INDENT at_least_one_stmt TOK_DEDENT
                            ;
at_least_one_stmt           :   at_least_one_stmt simple_stmt
                            |   at_least_one_stmt compound_stmt
                            |   simple_stmt
                            |   compound_stmt
                            ;


/* Undefined terms: INDENT DEDENT */
%%

int main(int argc, const char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }
    yyparse();

    return 0;
}