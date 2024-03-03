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

extern FILE* yyin;
extern stack<int> INDENT_STACK;

extern long long int NODE_COUNT;
extern node* AST_ROOT;

// void make_root_node() {
//     AST_ROOT = new node(FILE_INPUT, "root", false, NULL);
// }

void setup_dot() {
    dot_stream << "digraph ast {\n";
    // AST_ROOT->add_nodes_to_dot(dot_file);
    AST_ROOT->generate_dot_script();
    // AST_ROOT->add_edges_to_dot(dot_file);
    dot_stream << "}\n";

    ofstream out(filename);
    out << dot_stream.str();
    out.close();
}

int main(int argc, const char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }
    INDENT_STACK.push(0);
    yyparse();
    cout << "finished parsing" << endl;
    // AST_ROOT->traverse_tree();

    setup_dot();

    return 0;
}
