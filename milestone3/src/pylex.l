%option yylineno

%{
    #include <iostream>
    #include <stack>
    #include <string>
    #include <unistd.h>
    #include <utility>
    #include "pyparse.tab.h"
    using namespace std;

    extern FILE* yyin;
    stack<int> INDENT_STACK;
    int indent_space = 0, dedent_space = 0;
    int pending_dedent_flag = 0, pending_indent_flag = 0;
    extern int join_lines_implicitly;
    extern void yyerror (const char* s);
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
LEN                     "len"
NON_LOCAL               "nonlocal"
NOT                     "not"
OR                      "or"
PASS                    "pass"
PRINT                   "print"
RANGE                   "range"
RETURN                  "return"
SELF                    "self"
WHILE                   "while"

    // datatype
INT                     "int"
FLOAT                   "float"
STR                     "str"
BOOL                    "bool"
LIST                    "list"

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
    
SHORT_STRING_CHAR       [^"'\n\\]
LONG_STRING_CHAR        [^\\]

STRING_ESCAPE_SEQ       (\\\\)|(\\\')|(\\\")|(\\a)|(\\b)|(\\f)|(\\n)|(\\r)|(\\t)|(\\v)|(\\{OCT_DIGIT}{1,3})|(\\x{HEX_DIGIT}{2})
SHORT_STRING_ITEM       {SHORT_STRING_CHAR}|{STRING_ESCAPE_SEQ}
LONG_STRING_ITEM        {LONG_STRING_CHAR}|{STRING_ESCAPE_SEQ}
SHORT_STRING            \'{SHORT_STRING_ITEM}*\'|\"{SHORT_STRING_ITEM}*\"
LONG_STRING             \'{3}{LONG_STRING_ITEM}*\'{3}|\"{3}{LONG_STRING_ITEM}*\"{3}
STRING_LITERAL          {SHORT_STRING}|{LONG_STRING}

    // ERROR STRINGS
ERROR_SHORT_STRING      \"{SHORT_STRING_ITEM}*|\'{SHORT_STRING_ITEM}*
ERROR_LONG_STRING       \"{3}{SHORT_STRING_ITEM}*|\'{3}{SHORT_STRING_ITEM}*
    // TODO: possible correction here in ERROR_LONG_STRING

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
ERROR_CHAR              [$?`]
    //The following patterns have no rule matching them:
    //assert, from, 

%%
    {   // handling indent-dedent tokens
        if(pending_dedent_flag) {
            while(INDENT_STACK.top() > dedent_space) {
                INDENT_STACK.pop();
                return TOK_DEDENT;
            }
            if(INDENT_STACK.top() < dedent_space) {
                // cout << "pylex: indent_stack top = " << INDENT_STACK.top() << " and dedent_space " << dedent_space << endl;
                yyerror("IndentationError: unindent does not match any outer indentation level");
            }
            dedent_space = 0;
            pending_dedent_flag = 0;
        }
        if(pending_indent_flag) {
            INDENT_STACK.push(indent_space);
            indent_space = 0;
            pending_indent_flag = 0;
            return TOK_INDENT;
        }
    }

{COMMENT}               { ; }
{EXPLICIT_LINE_JOINER}  { ; }
{NEWLINE_WITH_SPACE}    {   int len = strlen(yytext) - 1;
                            if(!join_lines_implicitly) {
                                if(len % 2 != 0) {
                                    yyerror("IndentationError: incorrect indentation detected");
                                }
                                if(INDENT_STACK.top() == len) {
                                    // expected amount of space, do nothing
                                }
                                else if(INDENT_STACK.top() > len) {
                                    // a block has been closed, dedent must be passed
                                    dedent_space = len;
                                    pending_dedent_flag = 1;
                                }
                                else {
                                    // a block has been opened, indent must be passed
                                    indent_space = len;
                                    pending_indent_flag = 1;
                                }
                                return TOK_NEWLINE;
                            }
                        }

{EMPTY_LINES}           {   int len = strlen(yytext);
                            unput(yytext[len-1]); // this be equivalent to unput('\n')
                        }

{FALSE}                 { return TOK_FALSE; }
{TRUE}                  { return TOK_TRUE; }
{NONE}                  { return TOK_NONE; }
{AND}                   { return TOK_AND; }
{BREAK}                 { return TOK_BREAK; }
{CLASS}                 { return TOK_CLASS; }
{CONTINUE}              { return TOK_CONTINUE; }
{DEF}                   { return TOK_DEF; }
{ELIF}                  { return TOK_ELIF; }
{ELSE}                  { return TOK_ELSE; }
{FOR}                   { return TOK_FOR; }
{GLOBAL}                { return TOK_GLOBAL; }
{IF}                    { return TOK_IF; }
{IN}                    { return TOK_IN; }
{IS}                    { return TOK_IS; }
{LEN}                   { return TOK_LEN; }
{NON_LOCAL}             { return TOK_NON_LOCAL; }
{NOT}                   { return TOK_NOT; }
{OR}                    { return TOK_OR; }
{PASS}                  { return TOK_PASS; }
{WHILE}                 { return TOK_WHILE; }
{RETURN}                { return TOK_RETURN; }
{SELF}                  { return TOK_SELF; }
{PRINT}                 { return TOK_PRINT; }
{RANGE}                 { return TOK_RANGE; }

{INT}                   { return TOK_INT; }
{FLOAT}                 { return TOK_FLOAT; }
{STR}                   { return TOK_STR; }
{BOOL}                  { return TOK_BOOL; }
{LIST}                  { return TOK_LIST; }

{IDENTIFIER}            { return TOK_IDENTIFIER; }

{STRING_LITERAL}        { return TOK_STRING_LITERAL; }

{ERROR_SHORT_STRING}    { yyerror("SyntaxError: unterminated string literal"); }
{ERROR_LONG_STRING}     { yyerror("SyntaxError: unterminated triple-quoted string literal"); }

{INTEGER}               { return TOK_INTEGER_NUMBER; }
{FLOAT_NUMBER}          { return TOK_FLOAT_NUMBER; }
{IMAG_NUMBER}           { return TOK_IMAG_NUMBER; }

{PLUS}                  { return TOK_PLUS; }
{MINUS}                 { return TOK_MINUS; }
{STAR}                  { return TOK_STAR; }
{SLASH}                 { return TOK_SLASH; }
{DOUBLE_SLASH}          { return TOK_DOUBLE_SLASH; }
{PERCENT}               { return TOK_PERCENT; }
{DOUBLE_STAR}           { return TOK_DOUBLE_STAR; }

{EQ_EQUAL}              { return TOK_EQ_EQUAL; }
{NOT_EQUAL}             { return TOK_NOT_EQUAL; }
{GREATER}               { return TOK_GREATER; }
{LESS}                  { return TOK_LESS; }
{GREATER_EQUAL}         { return TOK_GREATER_EQUAL; }
{LESS_EQUAL}            { return TOK_LESS_EQUAL; }

{AMPER}                 { return TOK_AMPER; }
{VBAR}                  { return TOK_VBAR; }
{CIRCUMFLEX}            { return TOK_CIRCUMFLEX; }
{TILDE}                 { return TOK_TILDE; }
{LEFT_SHIFT}            { return TOK_LEFT_SHIFT; }
{RIGHT_SHIFT}           { return TOK_RIGHT_SHIFT; }

{EQUAL}                 { return TOK_EQUAL; }
{PLUS_EQUAL}            { return TOK_PLUS_EQUAL; }
{MINUS_EQUAL}           { return TOK_MINUS_EQUAL; }
{STAR_EQUAL}            { return TOK_STAR_EQUAL; }
{SLASH_EQUAL}           { return TOK_SLASH_EQUAL; }
{DOUBLE_SLASH_EQUAL}    { return TOK_DOUBLE_SLASH_EQUAL; }
{PERCENT_EQUAL}         { return TOK_PERCENT_EQUAL; }
{DOUBLE_STAR_EQUAL}     { return TOK_DOUBLE_STAR_EQUAL; }
{AMPER_EQUAL}           { return TOK_AMPER_EQUAL; }
{VBAR_EQUAL}            { return TOK_VBAR_EQUAL; }
{CIRCUMFLEX_EQUAL}      { return TOK_CIRCUMFLEX_EQUAL; }
{LEFT_SHIFT_EQUAL}      { return TOK_LEFT_SHIFT_EQUAL; }
{RIGHT_SHIFT_EQUAL}     { return TOK_RIGHT_SHIFT_EQUAL; }

{LPAR}                  { return TOK_LPAR; }
{RPAR}                  { return TOK_RPAR; }
{LSQB}                  { return TOK_LSQB; }
{RSQB}                  { return TOK_RSQB; }
{LBRACE}                { return TOK_LBRACE; }
{RBRACE}                { return TOK_RBRACE; }
{COMMA}                 { return TOK_COMMA; }
{COLON}                 { return TOK_COLON; }
{DOT}                   { return TOK_DOT; }
{SEMICOLON}             { return TOK_SEMICOLON; }
{RARROW}                { return TOK_RARROW; }
{ERROR_CHAR}            { yyerror("SyntaxError: invalid syntax"); }
.                       { ; }

%%
