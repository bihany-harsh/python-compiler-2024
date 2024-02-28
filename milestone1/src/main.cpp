#include <iostream>
#include <stack>
#include <string>
#include <fstream>
#include "pyparse.tab.h"
#include "include/node.hpp"
using namespace std;

string filename = "graph.dot";

extern FILE* yyin;
extern stack<int> INDENT_STACK;

extern long long int NODE_COUNT;
extern node* AST_ROOT;

void make_root_node() {
    AST_ROOT = new node(FILE_INPUT, "root", false, NULL);
}

void setup_dot(string filename) {
    string dot_file = "digraph ast {\n";
    // AST_ROOT->add_nodes_to_dot(dot_file);
    dot_file += "\n";
    // AST_ROOT->add_edges_to_dot(dot_file);
    dot_file += "}\n";

    ofstream out(filename);
    out << dot_file;
    out.close();
}

int main(int argc, const char** argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
    } else {
        yyin = stdin;
    }
    INDENT_STACK.push(0);

    make_root_node();

    setup_dot(filename);

    yyparse();

    return 0;
}
