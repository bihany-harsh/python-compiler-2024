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

%token TOK_NEWLINE
%token TOK_SEMICOLON TOK_COLON TOK_COMMA
%token TOK_IDENTIFIER TOK_NUMBER TOK_STRING_LITERAL
%token TOK_IF TOK_ELSE
%token TOK_NONE TOK_TRUE TOK_FALSE
%token TOK_AMPER TOK_VBAR TOK_CIRCUMFLEX
%token TOK_LEFT_SHIFT TOK_RIGHT_SHIFT
%token TOK_OR TOK_AND TOK_NOT
%token TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_PERCENT TOK_DOUBLE_SLASH
%token TOK_LPAR TOK_RPAR TOK_LSQB TOK_RSQB TOK_LBRACE TOK_RBRACE TOK_DOT
%token TOK_DOUBLE_STAR TOK_TILDE
%token TOK_EQUAL
%token TOK_LESS TOK_GREATER TOK_EQ_EQUAL TOK_GREATER_EQUAL TOK_LESS_EQUAL TOK_NOT_EQUAL TOK_IN TOK_IS
%token TOK_PLUS_EQUAL TOK_MINUS_EQUAL TOK_STAR_EQUAL TOK_SLASH_EQUAL TOK_PERCENT_EQUAL TOK_AMPER_EQUAL TOK_VBAR_EQUAL TOK_CIRCUMFLEX_EQUAL TOK_LEFT_SHIFT_EQUAL TOK_RIGHT_SHIFT_EQUAL TOK_DOUBLE_STAR_EQUAL TOK_DOUBLE_SLASH_EQUAL

%%

single_input                :   TOK_NEWLINE { cout << "single_input" << endl; }
                            |   simple_stmt
                            ;
simple_stmt                 :   small_stmt many_small_stmts optional_semicolon TOK_NEWLINE
                            ;
optional_semicolon          :   TOK_SEMICOLON
                            |   
                            ;
small_stmt                  :   expr_stmt /* TODO: many other types to be added */
                            ;
many_small_stmts            :   many_small_stmts TOK_SEMICOLON small_stmt
                            |
                            ;
expr_stmt                   :   testlist_star_expr annassign
                            |   testlist_star_expr augassign testlist
                            |   testlist_star_expr TOK_EQUAL many_testlist_star_expr
                            ;
testlist_star_expr          :   test
                            |   star_expr
                            ;
many_testlist_star_expr     :   many_testlist_star_expr testlist_star_expr
                            | 
                            ;
annassign                   :   TOK_COLON test optional_annassign_assign
                            ;
optional_annassign_assign   :   TOK_EQUAL test
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
testlist                    :   test many_comma_test optional_comma
                            ;
many_comma_test             :   many_comma_test TOK_COMMA test
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

                            /*TODO: Find out about vbar_or_or and amper_or_and */
or_test                     :   and_test many_or_and_test
                            ;
many_or_and_test            :   many_or_and_test vbar_or_or and_test
                            |
                            ;
vbar_or_or                  :   TOK_VBAR
                            |   TOK_OR
                            ;
and_test                    :   not_test many_and_not_test
                            ;
many_and_not_test           :   many_and_not_test amper_or_and not_test
                            |
                            ;
amper_or_and                :   TOK_AMPER
                            |   TOK_AND
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
expr                        :   xor_expr many_expr_xor_expr
                            ;
many_expr_xor_expr          :   many_expr_xor_expr TOK_VBAR xor_expr
                            |
                            ;
xor_expr                    :   and_expr many_xor_and_expr
                            ;
many_xor_and_expr           :   many_xor_and_expr TOK_CIRCUMFLEX and_expr
                            |
                            ;
and_expr                    :   shift_expr many_and_shift_expr
                            ;
many_and_shift_expr         :   many_and_shift_expr TOK_AMPER shift_expr
                            |
                            ;
shift_expr                  :   arith_expr many_shift_arith_expr
                            ;
many_shift_arith_expr       :   many_shift_arith_expr TOK_LEFT_SHIFT arith_expr
                            |   many_shift_arith_expr TOK_RIGHT_SHIFT arith_expr
                            |
                            ;   
arith_expr                  :   term many_arith_term
                            ;
many_arith_term             :   many_arith_term TOK_PLUS term
                            |   many_arith_term TOK_MINUS term
                            |
                            ;
term                        :   factor many_term_factor
                            ;
many_term_factor            :   many_term_factor TOK_STAR factor
                            |   many_term_factor TOK_SLASH factor
                            |   many_term_factor TOK_PERCENT factor
                            |   many_term_factor TOK_DOUBLE_SLASH factor
                            |
                            ;
factor                      :   TOK_PLUS factor
                            |   TOK_MINUS factor
                            |   TOK_TILDE factor
                            |   power
                            ;
power                       :   atom_expr optional_power_atom_expr
                            ;
optional_power_atom_expr    :   TOK_DOUBLE_STAR factor
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
                            //TODO: If to support ellipsis, add here
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
                                /* most probably redudant as represents *args, **kwags*/
                            |   TOK_DOUBLE_STAR test
                            |   TOK_STAR test
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
                            /* to define from exprlist */


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