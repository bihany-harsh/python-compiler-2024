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

    stack<int> INDENT_STACK;

    pair<int, int> check_indentation();
    pair<int, int> indent_res;

    // bool line_flag = false;

    extern FILE* yyin;

    void handle_indentation();
    void handle_eof();
    void token_handler();
    // int indent_count = 0;
    // int pending_indent_tokens = 0, pending_dedent_tokens = 0;
%}

NEWLINE                 [\n]
COMMENT                 #[.]*(\n)
EXPLICIT_LINE_JOINER    (\\)(\n)

INDENT_SPACE            ^(" ")+
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

    // %x  INDENT_DEDENT_STATE

%%
    /* Initialize a parser state object */
    yypstate* pstate = yypstate_new();

{INDENT_SPACE}          { handle_indentation(); }
{NEWLINE}               { token_handler(TOK_NEWLINE); }
{SEMICOLON}             { token_handler(TOK_SEMICOLON);  }
{EQUAL}                 { token_handler(TOK_EQUAL); }
{COLON}                 { token_handler(TOK_COLON); }
{COMMA}                 { token_handler(TOK_COMMA); }
{PLUS_EQUAL}            { token_handler(TOK_PLUS_EQUAL); }
{MINUS_EQUAL}           { token_handler(TOK_MINUS_EQUAL); }
{STAR_EQUAL}            { token_handler(TOK_STAR_EQUAL); }
{SLASH_EQUAL}           { token_handler(TOK_SLASH_EQUAL); }
{PERCENT_EQUAL}         { token_handler(TOK_PERCENT_EQUAL); }
{AMPER_EQUAL}           { token_handler(TOK_AMPER_EQUAL); }
{VBAR_EQUAL}            { token_handler(TOK_VBAR_EQUAL); }
{CIRCUMFLEX_EQUAL}      { token_handler(TOK_CIRCUMFLEX_EQUAL); }
{LEFT_SHIFT_EQUAL}      { token_handler(TOK_LEFT_SHIFT_EQUAL); }
{RIGHT_SHIFT_EQUAL}     { token_handler(TOK_RIGHT_SHIFT_EQUAL); }
{DOUBLE_STAR_EQUAL}     { token_handler(TOK_DOUBLE_STAR_EQUAL); }
{DOUBLE_SLASH_EQUAL}    { token_handler(TOK_DOUBLE_SLASH_EQUAL); }
{IF}                    { token_handler(TOK_IF); }
{ELSE}                  { token_handler(TOK_ELSE); }
{ELIF}                  { token_handler(TOK_ELIF); }
{WHILE}                 { token_handler(TOK_WHILE); }
{OR}                    { token_handler(TOK_OR); }
{AND}                   { token_handler(TOK_AND); }
{NOT}                   { token_handler(TOK_NOT); }
{AMPER}                 { token_handler(TOK_AMPER); }
{VBAR}                  { token_handler(TOK_VBAR); }
{CIRCUMFLEX}            { token_handler(TOK_CIRCUMFLEX); }
{LESS}                  { token_handler(TOK_LESS); }
{GREATER}               { token_handler(TOK_GREATER); }
{EQ_EQUAL}              { token_handler(TOK_EQ_EQUAL); }
{GREATER_EQUAL}         { token_handler(TOK_GREATER_EQUAL); }
{LESS_EQUAL}            { token_handler(TOK_LESS_EQUAL); }
{NOT_EQUAL}             { token_handler(TOK_NOT_EQUAL); }
{IN}                    { token_handler(TOK_IN); }
{IS}                    { token_handler(TOK_IS); }
{LEFT_SHIFT}            { token_handler(TOK_LEFT_SHIFT); }
{RIGHT_SHIFT}           { token_handler(TOK_RIGHT_SHIFT); }
{LPAR}                  { token_handler(TOK_LPAR); }
{RPAR}                  { token_handler(TOK_RPAR); }
{LSQB}                  { token_handler(TOK_LSQB); }
{RSQB}                  { token_handler(TOK_RSQB); }
{LBRACE}                { token_handler(TOK_LBRACE); }
{RBRACE}                { token_handler(TOK_RBRACE); }
{NONE}                  { token_handler(TOK_NONE); }
{TRUE}                  { token_handler(TOK_TRUE); }
{FALSE}                 { token_handler(TOK_FALSE); }
{NUMBER}                { token_handler(TOK_NUMBER); }
{DOT}                   { token_handler(TOK_DOT); }
{PASS}                  { token_handler(TOK_PASS); }
{BREAK}                 { token_handler(TOK_BREAK); }
{CONTINUE}              { token_handler(TOK_CONTINUE); }
{RETURN}                { token_handler(TOK_RETURN); }
{GLOBAL}                { token_handler(TOK_GLOBAL); }
{NON_LOCAL}             { token_handler(TOK_NON_LOCAL); }
{IDENTIFIER}            { token_handler(TOK_IDENTIFIER); }
<<EOF>>                 { handle_eof(); }
.                       { ; }

%%

void handle_eof() {
    while(INDENT_STACK.top() > 0) {
        INDENT_STACK.pop();
        yypush_parse(pstate, TOK_DEDENT, &yylval, &yylloc);
    }
    yypstate_delete(pstate);
}

void token_handler(int tok_) {
    yypush_parse(pstate, tok_, &yylval, &yylloc);
}

void handle_indentation() {
    int indent_level = strlen(yytext);
    if (indent_level%2 == 1) {
        cout << "TabError" << endl;
        // TODO: proper handling
        exit(-1);
    }
    indent_level /= 2;
    if (INDENT_STACK.top() < indent_level) {
        INDENT_STACK.push(indent_level);
        yypush_parse(pstate, TOK_INDENT, &yylval, &yylloc);
    } else {
        while (INDENT_STACK.top() > indent_level) {
            INDENT_STACK.pop();
            yypush_parse(pstate, TOK_DEDENT, &yylval, &yylloc);
        }
    }
}