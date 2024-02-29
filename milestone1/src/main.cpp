#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include "pyparse.tab.h"
#include "include/node.hpp"
using namespace std;

string filename = "graph.dot";
ostringstream dot_stream;

extern FILE* yyin;
extern stack<int> INDENT_STACK;

extern long long int NODE_COUNT;
extern node* AST_ROOT;

// void make_root_node() {
//     AST_ROOT = new node(FILE_INPUT, "root", false, NULL);
// }

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
    yyparse();

    cout << "main.cpp: AST_ROOT->children.size = " << AST_ROOT->children.size() << endl;
    for (auto child : AST_ROOT->children) {
        cout << "main.cpp: " << child->name << endl;
    }
    // AST_ROOT->traverse_tree();
    AST_ROOT->clean_tree();
    AST_ROOT->delete_delimiters();
    AST_ROOT->delete_single_child_nodes();

    dot_stream << "digraph ast {\n";
    AST_ROOT->generate_dot_script();
    dot_stream << "}\n";

    ofstream out(filename);
    out << dot_stream.str();
    out.close();


    return 0;
}