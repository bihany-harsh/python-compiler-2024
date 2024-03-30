#ifndef _3AC_HPP
#define _3AC_HPP
#include <string>
using namespace std;

typedef enum POS {
    OP,
    ARG1,
    ARG2,
    RESULT,
    LABEL,
} POS;
typedef enum quad_type {
    Q_UNARY,
    Q_BINARY,
    Q_COERCION,
    Q_ASSIGN,
    Q_DEREFERENCE,
    Q_PRINT,
    Q_JUMP,
    Q_COND_JUMP,
    Q_RETURN,
    Q_FUNC_CALL,

    Q_BLANK,
} quad_type;

const string IF_FALSE = "ifFalse ";

typedef struct Quadruple {
    string op;     // Operation
    string arg1;   // First argument
    string arg2;   // Second argument
    string result; // Result
    string label; // used to label the statements, relevant for jumping
    quad_type q_type;

    string code = "";   // Code

    Quadruple(const string& op, const string& arg1, const string& arg2, const string& result, quad_type q_type);

    string make_code();
    void rename_attribute(POS pos, string new_name); // op -> pos 0, arg1 -> pos 1, arg2 -> pos 2, result -> pos 3
} Quadruple;

void print_3AC(vector<Quadruple*> IR);
#endif