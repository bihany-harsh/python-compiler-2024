#ifndef _3AC_HPP
#define _3AC_HPP
#include <string>
using namespace std;

typedef enum quad_type {
    BINARY,
    UNARY,
    ASSIGN,
} quad_type;

struct Quadruple {
    string op;     // Operation
    string arg1;   // First argument
    string arg2;   // Second argument
    string result; // Result

    quad_type q_type;

    Quadruple(const string& op, const string& arg1, const string& arg2, const string& result, quad_type q_type);

    void gen_3ac_code();
};

#endif