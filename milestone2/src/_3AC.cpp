#include <iostream>
#include <string>
#include <vector>
#include "include/_3AC.hpp"
using namespace std;

vector<Quadruple*> INTERMEDIATE_CODE;
long long int INTERMEDIATE_COUNTER = 0; // the counter to generate intermediates like t1, t2 etc.

Quadruple::Quadruple(const string& op, const string& arg1, const string& arg2, const string& result, quad_type q_type) {
    this->op = op;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->result = result;
    this->q_type = q_type;

    this->code = this->make_code();
}

string Quadruple::make_code() {
    string code = "";
    switch(this->q_type) {
        case Q_BINARY:
            code = this->result + " = " + this->arg1 + " " + this->op + " " + this->arg2;
            break;
        case Q_UNARY:
            code = this->result + " = " + this->op + this->arg1;
            break;
        case Q_ASSIGN:
            code = this->result + " = " + this->arg1;
            break;
    }
    return code;
}

void print_3AC(vector<Quadruple*> INTERMEDIATE_CODE) {
    for(Quadruple* quad: INTERMEDIATE_CODE) {
        cout << quad->code << endl;
    }
}