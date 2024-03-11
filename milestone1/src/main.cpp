#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include "pyparse.tab.h"
#include "include/node.hpp"
using namespace std;

//TODO: Add support for command line arguments
const string filename = "graph.dot";
ostringstream dot_stream;
bool verbose_flag = false;

extern FILE* yyin;
extern stack<int> INDENT_STACK;

extern long long int NODE_COUNT;
extern node* AST_ROOT;

// void make_root_node() {
//     AST_ROOT = new node(FILE_INPUT, "root", false, NULL);
// }

void setup_dot() {
    dot_stream << "digraph ast {\n node [shape=rectangle]; \n";
    // AST_ROOT->add_nodes_to_dot(dot_file);
    AST_ROOT->generate_dot_script();
    // AST_ROOT->add_edges_to_dot(dot_file);
    dot_stream << "}\n";

    ofstream out(filename);
    out << dot_stream.str();
    out.close();
}

int main(int argc, const char** argv) {
    if(argc != 3) {
        cout << "Something went terribly wrong." << endl;
        exit(-1);
    }
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }
    if(string(argv[2]) == "true") {
        // verbose has been passed
        verbose_flag = true;
    }

    INDENT_STACK.push(0);
    if(verbose_flag) {
        cout << "Calling the parser routine..." << endl;
    }
    yyparse();
    if(verbose_flag) {
        cout << "Finished parsing!" << endl;
    }
    if(verbose_flag) {
        cout << "Generating dot file..." << endl;
    }
    setup_dot();
    if(verbose_flag) {
        cout << "Dot file generated!\nExiting the program..." << endl;
    }

    return 0;
}
