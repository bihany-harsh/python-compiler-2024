%option yylineno

%{
    #include <iostream>
    #include <stack>
    #include <string>
    #include <unistd.h>
    #include <utility>
    #include "pyparse.tab.h"
    using namespace std;
    // TODO: verify explicit line joining is being done correctly
    // TODO: ensure that empty lines will not cause an issue
    // TODO: Assert keyword?

    extern FILE* yyin;
    stack<int> INDENT_STACK;
    int indent_level = 0;
    int indent_space = 0, dedent_space = 0;
    int pending_dedent_flag = 0, pending_indent_flag = 0;
%}

NEWLINE                 [\n]
WHITE_SPACE             [ \t\r]*
NEWLINE_WITH_SPACE      {NEWLINE}(" ")*
EMPTY_LINES             ({NEWLINE}{WHITE_SPACE})*{NEWLINE}

PART_OF_LINE_COMMENT    #(.)*
ENTIRE_LINE_COMMENT     (\n){WHITE_SPACE}#(.)*
COMMENT                 {PART_OF_LINE_COMMENT}|{ENTIRE_LINE_COMMENT}
EXPLICIT_LINE_JOINER    (\\)(\n)

IDENTIFIER              [A-Za-z_][A-Za-z0-9_]*

    // keywords
FALSE                   "False"
NONE                    "None"
TRUE                    "True"
AND                     "and"
    // AS                      "as"
ASSERT                  "assert"
BREAK                   "break"
CLASS                   "class"
CONTINUE                "continue"
DEF                     "def"
ELIF                    "elif"
ELSE                    "else"
FOR                     "for"
    // FROM                    "from"
GLOBAL                  "global"
IF                      "if"
IN                      "in"
IS                      "is"
NON_LOCAL               "nonlocal"
NOT                     "not"
OR                      "or"
PASS                    "pass"
RETURN                  "return"
WHILE                   "while"

    // datatype
INT                     "int"
FLOAT                   "float"
STR                     "str"
BOOL                    "bool"

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
    
    // string and byte literals
SHORT_STRING_CHAR       [^"'\n\\]
LONG_STRING_CHAR        [^\\]

STRING_ESCAPE_SEQ       (\\\\)|(\\\')|(\\\")|(\\a)|(\\b)|(\\f)|(\\n)|(\\r)|(\\t)|(\\v)|(\\{OCT_DIGIT}{1,3})|(\\x{HEX_DIGIT}{2})
SHORT_STRING_ITEM       {SHORT_STRING_CHAR}|{STRING_ESCAPE_SEQ}
LONG_STRING_ITEM        {LONG_STRING_CHAR}|{STRING_ESCAPE_SEQ}
SHORT_STRING            \'{SHORT_STRING_ITEM}*\'|\"{SHORT_STRING_ITEM}*\"
LONG_STRING             \'{3}{LONG_STRING_ITEM}*\'{3}|\"{3}{LONG_STRING_ITEM}*\"{3}
STRING_LITERAL          {SHORT_STRING}|{LONG_STRING}

    //TODO: verify that no escape characters to be supported
    //TODO: byte literals?


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
    //TODO: printing ASCII characters: what to do?
ERROR_CHAR              [$?`]
    //The following patterns have no rule matching them:
    //assert, from, 

%%
    {   // handling indent-dedent tokens
        if(pending_dedent_flag) {
            while(INDENT_STACK.top() > dedent_space) {
                INDENT_STACK.pop();
                cout << "pylex: returning TOK_DEDENT" << endl;
                return TOK_DEDENT;
            }
            if(INDENT_STACK.top() < dedent_space) {
                //TODO: Throw error, unexpected dedent
            }
            dedent_space = 0;
            pending_dedent_flag = 0;
        }
        if(pending_indent_flag) {
            INDENT_STACK.push(indent_space);
            cout << "pylex: returning TOK_INDENT" << endl;
            indent_space = 0;
            pending_indent_flag = 0;
            return TOK_INDENT;
        }
    }

{COMMENT}               { cout << "pylex: ignoring comment" << yytext << endl;}
{EXPLICIT_LINE_JOINER}  { cout << "pylex: found an explicit line joiner, doing nothing" << endl; }
{NEWLINE_WITH_SPACE}    {   cout << "top of the stack now is " << INDENT_STACK.top() << endl;
                            cout << "found newline (maybe with space)" << endl;
                            int len = strlen(yytext) - 1;
                            // if(joining_line_implicitly) {
                                // //TODO: simply ignore
                            // }
                            if(len % 2 != 0) {
                                //TODO: throw error, odd number of spaces
                            }
                            if(INDENT_STACK.top() == len) {
                                // expected amount of space, do nothing
                                cout << "pylex: indent check: correct amount of space" << endl;
                            }
                            else if(INDENT_STACK.top() > len) {
                                // a block has been closed, dedent must be passed
                                dedent_space = len;
                                // if(dedent_space < INDENT_STACK.top()) {
                                //     cout << "need to pass multiple dedents" << endl;
                                //     pending_dedent_flag = 1;
                                // }
                                pending_dedent_flag = 1;
                                cout << "pylex: indent check: found that dedent is required, will pass dedent from now" << endl;
                            }
                            else {
                                // a block has been opened, indent must be passed
                                indent_space = len;
                                pending_indent_flag = 1;
                                cout << "pylex: indent check: found that indent is required, will pass indent" << endl;
                            }
                            cout << "pylex: indent check: returning TOK_NEWLINE" << endl;
                            return TOK_NEWLINE;
                        }

{EMPTY_LINES}           {   cout << "pylex: found empty lines, doing nothing" << endl;
                            int len = strlen(yytext);
                            unput(yytext[len-1]); // this be equivalent to unput('\n')
                            // return TOK_NEWLINE;
                        }

{FALSE}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_FALSE; }
{TRUE}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_TRUE; }
{NONE}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_NONE; }
{AND}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_AND; }
{BREAK}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_BREAK; }
{CLASS}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_CLASS; }
{CONTINUE}              { cout << "pylex: found keyword " << yytext << endl; return TOK_CONTINUE; }
{DEF}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_DEF; }
{ELIF}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_ELIF; }
{ELSE}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_ELSE; }
{FOR}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_FOR; }
{GLOBAL}                { cout << "pylex: found keyword " << yytext << endl; return TOK_GLOBAL; }
{IF}                    { cout << "pylex: found keyword " << yytext << endl; return TOK_IF; }
{IN}                    { cout << "pylex: found keyword " << yytext << endl; return TOK_IN; }
{IS}                    { cout << "pylex: found keyword " << yytext << endl; return TOK_IS; }
{NON_LOCAL}             { cout << "pylex: found keyword " << yytext << endl; return TOK_NON_LOCAL; }
{NOT}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_NOT; }
{OR}                    { cout << "pylex: found keyword " << yytext << endl; return TOK_OR; }
{PASS}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_PASS; }
{WHILE}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_WHILE; }
{RETURN}                { cout << "pylex: found keyword " << yytext << endl; return TOK_RETURN; }

{INT}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_INT; }
{FLOAT}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_FLOAT; }
{STR}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_STR; }
{BOOL}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_BOOL; }

{IDENTIFIER}            { cout << "pylex: found keyword " << yytext << endl; return TOK_IDENTIFIER; }

{STRING_LITERAL}        { cout << "pylex: found string " << yytext << endl; return TOK_STRING_LITERAL; }

{NUMBER}                { cout << "pylex: found integer " << yytext << endl; return TOK_NUMBER; }

{PLUS}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_PLUS; }
{MINUS}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_MINUS; }
{STAR}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_STAR; }
{SLASH}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_SLASH; }
{DOUBLE_SLASH}          { cout << "pylex: found keyword " << yytext << endl; return TOK_DOUBLE_SLASH; }
{PERCENT}               { cout << "pylex: found keyword " << yytext << endl; return TOK_PERCENT; }
{DOUBLE_STAR}           { cout << "pylex: found keyword " << yytext << endl; return TOK_DOUBLE_STAR; }

{EQ_EQUAL}              { cout << "pylex: found keyword " << yytext << endl; return TOK_EQ_EQUAL; }
{NOT_EQUAL}             { cout << "pylex: found keyword " << yytext << endl; return TOK_NOT_EQUAL; }
{GREATER}               { cout << "pylex: found keyword " << yytext << endl; return TOK_GREATER; }
{LESS}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_LESS; }
{GREATER_EQUAL}         { cout << "pylex: found keyword " << yytext << endl; return TOK_GREATER_EQUAL; }
{LESS_EQUAL}            { cout << "pylex: found keyword " << yytext << endl; return TOK_LESS_EQUAL; }

{AMPER}                 { cout << "found keyword " << yytext << endl; return TOK_AMPER; }
{VBAR}                  { cout << "found keyword " << yytext << endl; return TOK_VBAR; }
{CIRCUMFLEX}            { cout << "found keyword " << yytext << endl; return TOK_CIRCUMFLEX; }
{TILDE}                 { cout << "found keyword " << yytext << endl; return TOK_TILDE; }
{LEFT_SHIFT}            { cout << "found keyword " << yytext << endl; return TOK_LEFT_SHIFT; }
{RIGHT_SHIFT}           { cout << "found keyword " << yytext << endl; return TOK_RIGHT_SHIFT; }

{EQUAL}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_EQUAL; }
{PLUS_EQUAL}            { cout << "pylex: found keyword " << yytext << endl; return TOK_PLUS_EQUAL; }
{MINUS_EQUAL}           { cout << "pylex: found keyword " << yytext << endl; return TOK_MINUS_EQUAL; }
{STAR_EQUAL}            { cout << "pylex: found keyword " << yytext << endl; return TOK_STAR_EQUAL; }
{SLASH_EQUAL}           { cout << "pylex: found keyword " << yytext << endl; return TOK_SLASH_EQUAL; }
{DOUBLE_SLASH_EQUAL}    { cout << "pylex: found keyword " << yytext << endl; return TOK_DOUBLE_SLASH_EQUAL; }
{PERCENT_EQUAL}         { cout << "pylex: found keyword " << yytext << endl; return TOK_PERCENT_EQUAL; }
{DOUBLE_STAR_EQUAL}     { cout << "pylex: found keyword " << yytext << endl; return TOK_DOUBLE_STAR_EQUAL; }
{AMPER_EQUAL}           { cout << "pylex: found keyword " << yytext << endl; return TOK_AMPER_EQUAL; }
{VBAR_EQUAL}            { cout << "pylex: found keyword " << yytext << endl; return TOK_VBAR_EQUAL; }
{CIRCUMFLEX_EQUAL}      { cout << "pylex: found keyword " << yytext << endl; return TOK_CIRCUMFLEX_EQUAL; }
{LEFT_SHIFT_EQUAL}      { cout << "pylex: found keyword " << yytext << endl; return TOK_LEFT_SHIFT_EQUAL; }
{RIGHT_SHIFT_EQUAL}     { cout << "pylex: found keyword " << yytext << endl; return TOK_RIGHT_SHIFT_EQUAL; }

{LPAR}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_LPAR; }
{RPAR}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_RPAR; }
{LSQB}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_LSQB; }
{RSQB}                  { cout << "pylex: found keyword " << yytext << endl; return TOK_RSQB; }
{LBRACE}                { cout << "pylex: found keyword " << yytext << endl; return TOK_LBRACE; }
{RBRACE}                { cout << "pylex: found keyword " << yytext << endl; return TOK_RBRACE; }
{COMMA}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_COMMA; }
{COLON}                 { cout << "pylex: found keyword " << yytext << endl; return TOK_COLON; }
{DOT}                   { cout << "pylex: found keyword " << yytext << endl; return TOK_DOT; }
{SEMICOLON}             { cout << "pylex: found keyword " << yytext << endl; return TOK_SEMICOLON; }
{RARROW}                { cout << "pylex: found keyword " << yytext << endl; return TOK_RARROW; }
.                       { ; }

%%

// int main() {
//     yylex();

//     return 0;
// }