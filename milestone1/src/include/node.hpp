// to define the node structure used in the Abstract Syntax Tree (AST)
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef enum {
    FILE_INPUT,
} node_type;

typedef struct node {
    unsigned long long int ID; // ID for a node
    node_type type;
    string name = "";
    bool is_terminal = false;

    // the value of the node (assuming the primitive types are string, int, double, bool)
    string str_val = "";
    long long int int_val = 0;
    double double_val = 0.0;
    bool bool_val = false;
    string val_repr = ""; // the string representation of the value

    // line no
    int line_no = 0;

    // parent and children
    vector<node*> children;
    node* parent;

    // METHODS
    // constructor
    node(node_type, string name, bool is_terminal, node* parent);
    // functional overloading to assign value to the node
    void assign_value(string value);
    void assign_value(long long int value);
    void assign_value(double value);
    void assign_value(bool value);
    // add a child
    void add_child(node* child);

    // to dot file
    void add_nodes_to_dot(string& dot_file);
    void add_edges_to_dot(string& dot_file);
} node;
