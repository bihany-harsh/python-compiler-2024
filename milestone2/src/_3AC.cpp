#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "include/_3AC.hpp"
using namespace std;

vector<Quadruple*> IR;
long long int INTERMEDIATE_COUNTER = 1; // the counter to generate intermediates like t1, t2 etc.
stack<int> LABEL_CNT_STACK; // stack to keep track of the label count
int LABEL_COUNTER = 1; // the counter to generate labels like L1, L2 etc.

Quadruple::Quadruple(const string& op, const string& arg1, const string& arg2, const string& result, quad_type q_type) {
    this->op = op;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->result = result;
    this->q_type = q_type;
    this->label = LABEL_COUNTER++;

    this->code = this->make_code();
}

string Quadruple::make_code() {
    string code = "";
    switch(this->q_type) {
        case Q_UNARY:
            code = to_string(this->label) + ":\t\t" + this->result + " = " + this->op + " " + this->arg1;
            break;
        case Q_BINARY:
            code = to_string(this->label) + ":\t" + this->result + " = " + this->arg1 + " " + this->op + " " + this->arg2;
            break;
        case Q_COERCION:
            code = to_string(this->label) + ":\t" + this->result + " = (" + this->arg1 + ") " + this->arg2;
            break;
        case Q_ASSIGN:
            code = to_string(this->label) + ":\t" + this->result + " = " + this->arg1;
            break;
        case Q_DEREFERENCE:
            code = to_string(this->label) + ":\t" + this->result + " = *" + this->arg1;
            break;
        case Q_PRINT:
            code = to_string(this->label) + ":\tprint " + this->arg1;
            break;
        case Q_JUMP:
            code = to_string(this->label) + ":\tgoto " + this->result;
            break;
        case Q_COND_JUMP:
            code = to_string(this->label) + ":\t" + IF_FALSE + this->arg1 + " " + " goto " + this->result;
            break;
        case Q_BLANK:
            code = to_string(this->label) + ":\t";
            break;
    }
    return code;
}

void Quadruple::rename_attribute(POS code, string new_name) {
    switch(code) {
        case 0:
            this->op = new_name;
            break;
        case 1:
            this->arg1 = new_name;
            break;
        case 2:
            this->arg2 = new_name;
            break;
        case 3:
            this->result = new_name;
            break;
    }
    this->code = this->make_code();
}

void print_3AC(vector<Quadruple*> IR) {
    for(Quadruple* quad: IR) {
        cout << quad->code << endl;
    }
}