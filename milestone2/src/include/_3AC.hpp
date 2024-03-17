#ifndef _3AC_HPP
#define _3AC_HPP
#include <string>
using namespace std;

typedef enum quad_type {
    Q_BINARY,
    Q_UNARY,
    Q_ASSIGN,
    Q_INDEX, // a statement where list element is indexed
    Q_PRINT,
} quad_type;

typedef struct Quadruple {
    string op;     // Operation
    string arg1;   // First argument
    string arg2;   // Second argument
    string result; // Result
    quad_type q_type;

    string code = "";   // Code

    Quadruple(const string& op, const string& arg1, const string& arg2, const string& result, quad_type q_type);

    string make_code();
} Quadruple;

void print_3AC(vector<Quadruple*> INTERMEDIATE_CODE);
#endif