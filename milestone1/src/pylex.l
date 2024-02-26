%option yylineno

%{
    #include <iostream>
    #include <stack>
    #include <string>
    #include <unistd.h>
    #include <utility>
    #include "pyparse.tab.h"
    using namespace std;
    // explicit line joining may be a problem
    // TODO: Assert keyword?

    extern FILE* yyin;
    int indent_count = 0;
    int pending_indent_tokens = 0, pending_dedent_tokens = 0;
%}

NEWLINE                 [\n]
COMMENT                 #[.]*(\n)
EXPLICIT_LINE_JOINER    (\\)(\n)

NEWLINE_WITH_SPACE      [\n]("  ")*
IDENTIFIER              [A-Za-z_][A-Za-z0-9_]*

    // keywords
FALSE                   "False"
NONE                    "None"
TRUE                    "True"
AND                     "and"
AS                      "as"
ASSERT                  "assert"
BREAK                   "break"
CLASS                   "class"
CONTINUE                "continue"
DEF                     "def"
ELIF                    "elif"
ELSE                    "else"
EXCEPT                  "except"
FINALLY                 "finally"
FOR                     "for"
FROM                    "from"
GLOBAL                  "global"
IF                      "if"
IN                      "in"
IS                      "is"
NON_LOCAL               "nonlocal"
NOT                     "not"
OR                      "or"
PASS                    "pass"
RETURN                  "return"
TRY                     "try"
WHILE                   "while"

    // string and byte literals
SHORT_STRING_CHAR       [^"'\n\\]
LONG_STRING_CHAR        [^\\]
STRING_ESCAPE_SEQ       (\\)/[.]
SHORT_STRING_ITEM       {SHORT_STRING_CHAR}|{STRING_ESCAPE_SEQ}
LONG_STRING_ITEM        {LONG_STRING_CHAR}|{STRING_ESCAPE_SEQ}
SHORT_STRING            \'{SHORT_STRING_ITEM}*\'|\"{SHORT_STRING_ITEM}*\"
LONG_STRING             \'{3}{LONG_STRING_ITEM}*\'{3}|\"{3}{LONG_STRING_ITEM}*\"{3}
STRING_LITERAL          {SHORT_STRING}|{LONG_STRING}
    //FIXME: string literal not working

    //TODO: verify that no byte prefix is required
    //TODO: verify that no escape characters to be supported
    //TODO: verify whether "await" has to be supported

    // integer literals
OCT_DIGIT               [0-7]
BIN_DIGIT               [0-1]
DIGIT                   [0-9]
NONZERO_DIGIT           [1-9]
HEX_DIGIT               {DIGIT}|[a-fA-f]
HEX_INTEGER             "0"("X"|"x")(("_")?{HEX_DIGIT})+
OCT_INTEGER             "0"("O"|"o")(("_")?{OCT_DIGIT})+
BIN_INTEGER             "0"("B"|"b")(("_")?{BIN_DIGIT})+
DEC_INTEGER             {NONZERO_DIGIT}(("_")?{DIGIT})*|"0"+(("_")?"0")*
    //TODO: test DEC_INTEGER
INTEGER                 {DEC_INTEGER}|{BIN_INTEGER}|{OCT_INTEGER}|{HEX_INTEGER}

    // floating point literals
DIGIT_PART              {DIGIT}(("_")?{DIGIT})*
FRACTION                "."{DIGIT_PART}
EXPONENT                ("E"|"e")[+-]?{DIGIT_PART}
POINT_FLOAT             {DIGIT_PART}?{FRACTION}|{DIGIT_PART}"."
EXPONENT_FLOAT          ({DIGIT_PART}|{POINT_FLOAT}){EXPONENT}
FLOAT_NUMBER            {POINT_FLOAT}|{EXPONENT_FLOAT}

    // imaginary literals
IMAG_NUMBER             ({FLOAT_NUMBER}|{DIGIT_PART})("j"|"J")

NUMBER                  {INTEGER}|{FLOAT_NUMBER}|{IMAG_NUMBER}

    // operators
    //arithmetic operators
PLUS                    "+"
MINUS                   "-"
STAR                    "*"
SLASH                   "/"
DOUBLE_SLASH            "//"
PERCENT                 "%"
DOUBLE_STAR             "**"
    //relational operators
EQ_EQUAL                "=="
NOT_EQUAL               "!="
GREATER                 ">"
LESS                    "<"
GREATER_EQUAL           ">="
LESS_EQUAL              "<="
    //bitwise operators
AMPER                   "&"
VBAR                    "|"
CIRCUMFLEX              "^"
TILDE                   "~"
LEFT_SHIFT              "<<"
RIGHT_SHIFT             ">>"
    //assignment operators
EQUAL                   "="
PLUS_EQUAL              "+="
MINUS_EQUAL             "-="
STAR_EQUAL              "*="
SLASH_EQUAL             "/="
DOUBLE_SLASH_EQUAL      "//="
PERCENT_EQUAL           "%="
DOUBLE_STAR_EQUAL       "**="
AMPER_EQUAL             "&="
VBAR_EQUAL              "|="
CIRCUMFLEX_EQUAL        "^="
LEFT_SHIFT_EQUAL        "<<="
RIGHT_SHIFT_EQUAL       ">>="

    // delimiters
LPAR                    "("
RPAR                    ")"
LSQB                    "["
RSQB                    "]"
LBRACE                  "{"
RBRACE                  "}"
COMMA                   ","
COLON                   ":"
DOT                     "."
SEMICOLON               ";"
RARROW                  "->"
    //TODO: Find out whether to include ellipsis literals or not
    //TODO: printing ASCII characters: what to do?
ERROR_CHAR              [$?`]

%x  INDENT_DEDENT_STATE

%%

{NEWLINE_WITH_SPACE}    {   int len = (strlen(yytext) - 1) / 2;
                            if(indent_count == len) {
                                // expected amount of space, simply return
                            }
                            else if(indent_count > len) {
                                // a block has been closed, dedent must be passed
                                pending_dedent_tokens = indent_count - len;
                                indent_count = len;
                                cout << "243. beginning the indent-dedent space" << endl;
                                BEGIN(INDENT_DEDENT_STATE);
                            }
                            else {
                                // a block has been opened, indent must be passed
                                pending_indent_tokens = len - indent_count;
                                indent_count = len;
                                cout << "250. beginning the indent-dedent space" << endl;
                                BEGIN(INDENT_DEDENT_STATE);
                            }
                            cout << "253. returning TOK_NEWLINE" << endl;
                            return TOK_NEWLINE;
                        }
<INDENT_DEDENT_STATE>.  {   unput(yytext[0]);
                            while(pending_dedent_tokens) {
                                pending_dedent_tokens--;
                                cout << "259. returning TOK_DEDENT" << endl;
                                return TOK_DEDENT;
                            }
                            while(pending_indent_tokens) {
                                pending_indent_tokens--;
                                cout << "264. returning TOK_INDENT" << endl;
                                return TOK_INDENT;
                            }
                            BEGIN(INITIAL);
                        }
{SEMICOLON}             { return TOK_SEMICOLON; }
{EQUAL}                 { return TOK_EQUAL; }
{COLON}                 { return TOK_COLON; }
{COMMA}                 { return TOK_COMMA; }
{PLUS_EQUAL}            { return TOK_PLUS_EQUAL; }
{MINUS_EQUAL}           { return TOK_MINUS_EQUAL; }
{STAR_EQUAL}            { return TOK_STAR_EQUAL; }
{SLASH_EQUAL}           { return TOK_SLASH_EQUAL; }
{PERCENT_EQUAL}         { return TOK_PERCENT_EQUAL; }
{AMPER_EQUAL}           { return TOK_AMPER_EQUAL; }
{VBAR_EQUAL}            { return TOK_VBAR_EQUAL; }
{CIRCUMFLEX_EQUAL}      { return TOK_CIRCUMFLEX_EQUAL; }
{LEFT_SHIFT_EQUAL}      { return TOK_LEFT_SHIFT_EQUAL; }
{RIGHT_SHIFT_EQUAL}     { return TOK_RIGHT_SHIFT_EQUAL; }
{DOUBLE_STAR_EQUAL}     { return TOK_DOUBLE_STAR_EQUAL; }
{DOUBLE_SLASH_EQUAL}    { return TOK_DOUBLE_SLASH_EQUAL; }
{IF}                    { return TOK_IF; }
{ELSE}                  { return TOK_ELSE; }
{ELIF}                  { return TOK_ELIF; }
{WHILE}                 { return TOK_WHILE; }
{OR}                    { return TOK_OR; }
{AND}                   { return TOK_AND; }
{NOT}                   { return TOK_NOT; }
{AMPER}                 { return TOK_AMPER; }
{VBAR}                  { return TOK_VBAR; }
{CIRCUMFLEX}            { return TOK_CIRCUMFLEX; }
{LESS}                  { return TOK_LESS; }
{GREATER}               { return TOK_GREATER; }
{EQ_EQUAL}              { return TOK_EQ_EQUAL; }
{GREATER_EQUAL}         { return TOK_GREATER_EQUAL; }
{LESS_EQUAL}            { return TOK_LESS_EQUAL; }
{NOT_EQUAL}             { return TOK_NOT_EQUAL; }
{IN}                    { return TOK_IN; }
{IS}                    { return TOK_IS; }
{LEFT_SHIFT}            { return TOK_LEFT_SHIFT; }
{RIGHT_SHIFT}           { return TOK_RIGHT_SHIFT; }
{LPAR}                  { return TOK_LPAR; }
{RPAR}                  { return TOK_RPAR; }
{LSQB}                  { return TOK_LSQB; }
{RSQB}                  { return TOK_RSQB; }
{LBRACE}                { return TOK_LBRACE; }
{RBRACE}                { return TOK_RBRACE; }
{NONE}                  { return TOK_NONE; }
{TRUE}                  { return TOK_TRUE; }
{FALSE}                 { return TOK_FALSE; }
{NUMBER}                { return TOK_NUMBER; }
{DOT}                   { return TOK_DOT; }
{PASS}                  { return TOK_PASS; }
{BREAK}                 { return TOK_BREAK; }
{CONTINUE}              { return TOK_CONTINUE; }
{RETURN}                { return TOK_RETURN; }
{GLOBAL}                { return TOK_GLOBAL; }
{NON_LOCAL}             { return TOK_NON_LOCAL; }
{IDENTIFIER}            { cout << "returning token " << yytext << endl; return TOK_IDENTIFIER; }
.                       { ; }

%%

// int main(int argc, const char** argv) {
//     if (argc > 1) {
//         yyin = fopen(argv[1], "r");
//     } else {
//         yyin = stdin;
//     }
//     yylex();

//     return 0;
// }