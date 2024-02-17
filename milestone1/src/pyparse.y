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
%token TOK_EQUAL
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
                            /* start from defining `test` and `star_expr` */

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