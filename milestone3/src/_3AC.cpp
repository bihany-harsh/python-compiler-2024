#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <fstream>
#include "include/_3AC.hpp"
using namespace std;

vector<Quadruple*> IR;
long long int INTERMEDIATE_COUNTER = 1; // the counter to generate intermediates like t1, t2 etc.
stack<int> LABEL_CNT_STACK; // stack to keep track of the label count
int LABEL_COUNTER = 1; // the counter to generate labels like L1, L2 etc.
bool pending_return = false; // it is set to true when a function is started and the return statement has not been encountered yet
// if at the end of a function (not returning void), this is true, then an error to be thrown
bool pending_init = false;

set<int> targets;

Quadruple::Quadruple(const string& op, const string& arg1, const string& arg2, const string& result, quad_type q_type) {
    this->op = op;
    this->arg1 = arg1;
    this->arg2 = arg2;
    this->result = result;
    this->q_type = q_type;
    this->label = to_string(LABEL_COUNTER++);

    this->code = this->make_code();
}

string Quadruple::make_code() {
    string code = "";
    switch(this->q_type) {
        case Q_UNARY:
            code = this->label + ":\t" + this->result + " = " + this->op + " " + this->arg1;
            break;
        case Q_BINARY:
            code = this->label + ":\t" + this->result + " = " + this->arg1 + " " + this->op + " " + this->arg2;
            break;
        case Q_BINARY_STR:
            code = this->label + ":\t" + this->result + " = " + this->arg1 + " " + this->op + " " + this->arg2;
            break;
        case Q_COERCION:
            code = this->label + ":\t" + this->result + " = (" + this->arg1 + ") " + this->arg2;
            break;
        case Q_ASSIGN:
            code = this->label + ":\t" + this->result + " = " + this->arg1;
            break;
        case Q_DEREFERENCE:
            code = this->label + ":\t" + this->result + " = *" + this->arg1;
            break;
        case Q_STORE:
            code = this->label + ":\t*" + this->result + " = " + this->arg1;
            break;
        case Q_PRINT:
            code = this->label + ":\tprint " + this->arg1;
            break;
        case Q_PRINT_STR:
            code = this->label + ":\tprint " + this->arg1;
            break;
        case Q_JUMP:
            code = this->label + ":\t" + this->op + " " + this->result; // handles both goto and return (unconditional jumps)
            break;
        case Q_COND_JUMP:
            code = this->label + ":\t" + IF_FALSE + this->arg1 + " " + " goto " + this->result;
            break;
        case Q_FUNC_CALL:
            code = this->label + ":\t" + this->result + " = call " + this->arg1 + ", " + this->arg2;
            break;
        case Q_PUSH_PARAM:
            code = this->label + ":\tpush_param " + this->result;
            break;
        case Q_POP_PARAM:
            code = this->label + ":\t" + this->result + " = pop_param";
            break;
        case Q_ALLOC:
            code = this->label + ":\t" + this->result + " = alloc " + this->arg1;
            break;
        case Q_FUNC_LABEL:
            code = this->label + ":\t" + this->arg1 + " " + this->arg2;
            break;
        case Q_SP_UPDATE:
            code = this->label + ":\tstackpointer " + this->arg1;
            break;
        case Q_BLANK:
            code = this->label + ":\t";
            break;
    }
    return code;
}

void Quadruple::rename_attribute(POS code, string new_name) {
    switch(code) {
        case OP:
            this->op = new_name;
            break;
        case ARG1:
            this->arg1 = new_name;
            break;
        case ARG2:
            this->arg2 = new_name;
            break;
        case RESULT:
            this->result = new_name;
            break;
        case LABEL:
            this->label = new_name;
            break;
    }
    this->code = this->make_code();
}

void print_3AC(vector<Quadruple*> IR) {
    for(Quadruple* quad: IR) {
        cout << quad->code << endl;
    }
}

void output_3AC_to_txt(const string& filename) {
    if (filename != "") {
        ofstream txtFile(filename);
        if (!txtFile.is_open()) {
            std::cerr << "Failed to open file for writing.\n";
            exit(1);
        }
        for(Quadruple* q: IR) {
            txtFile << q->code << endl;
            if (targets.find(atoi(q->label.c_str())) != targets.end()){
                // cout << q->code << endl;
                q->is_target = true;
            }
        }
    } else {
        for(Quadruple* q: IR) {
            if (targets.find(atoi(q->label.c_str())) != targets.end()){
                // cout << q->code << endl;
                q->is_target = true;
            }
        }
    }
    
    
}