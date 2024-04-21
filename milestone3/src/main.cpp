#include <iostream>
#include <stack>
#include <string>
#include <sstream>
#include <fstream>
#include "pyparse.tab.h"
#include "include/node.hpp"
#include "include/symbol_table.hpp"
#include "include/x86.hpp"
using namespace std;

string dot_filename = "graph.dot";
ostringstream dot_stream;
bool verbose_flag = false;

string x86_filename = "x86_code.s";
string sym_csv = "symbol_table.csv";

extern FILE* yyin;
extern stack<int> INDENT_STACK;

extern long long int NODE_COUNT;
extern node* AST_ROOT;

extern symbol_table* SYMBOL_TABLE;
extern stack<symbol_table*> ST_STACK;
extern st_entry* dummy_entry;
extern stack<int> OFFSET_STACK;
extern int OFFSET;
extern ofstream csvFile;

extern vector<Quadruple*> IR;
extern stack<int> LABEL_CNT_STACK;

string input_file = "../tests/test1.py";
string tac_file = "tac.txt";

void setup_dot() {
    dot_stream << "digraph ast {\n node [shape=rectangle]; \n";
    // AST_ROOT->add_nodes_to_dot(dot_file);
    AST_ROOT->generate_dot_script();
    // AST_ROOT->add_edges_to_dot(dot_file);
    dot_stream << "}\n";

    ofstream out(dot_filename);
    out << dot_stream.str();
    out.close();
}

void print_help_page() {
    cout << "Usage:     \n\n";
    cout << "Commands:\n-h, --help \t\t\t\t\t Show help page\n";
    cout << "-i, -input <input_file_name> \t\t\t Give input file\n";
    cout << "-o, -output <output_file_name>\t\t\t Redirect dot file to output file\n";
    cout << "-t, -tac <tac_file_name>\t\t\t Redirect 3AC file to tac output file\n";
    cout << "-a, -asm <assembly_file_name>]\t\t\t Redirect assembly code to assembly output file\n";
    cout << "-s, -sym \t\t\t\t\t Outputs the entire derivation in command line\n";
    cout << "-v, -verbose \t\t\t\t\t\n";
    return;
}


int main(int argc, const char** argv) {
    // cout << "Execution started" << endl;

    for(int i = 1; i < argc; i++){
        if(string(argv[i]) == "-help" || string(argv[i]) == "-h") {
            print_help_page();
            return -1;
        }
        else if(std::string(argv[i]) == "-input" || std::string(argv[i]) == "-i") {
            if((i + 1) < argc) input_file = argv[i+1];
            else cout << "Error: No input filename given";
            i++;
        }
        else if(std::string(argv[i]) == "-output" || std::string(argv[i]) == "-o") {
            if((i + 1) < argc) dot_filename = argv[i+1];
            else cout << "Error: No output filename given";
            i++;
        }
        else if(std::string(argv[i]) == "-tac" || std::string(argv[i]) == "-t") {
            if((i + 1) < argc) tac_file = argv[i+1];
            else cout << "Error: No 3AC filename given";
            i++;
        }
        else if(std::string(argv[i]) == "-verbose" || std::string(argv[i]) == "-v") {
            verbose_flag = true;
        }
        else if(std::string(argv[i]) == "-asm" || std::string(argv[i]) == "-a") {
            if((i + 1) < argc) x86_filename = argv[i+1];
            else cout << "Error: No assembly filename given";
            i++;
        }
        else if (string(argv[i]) == "-s" || string(argv[i]) == "-sym") {
            if ((i + 1) < argc) sym_csv = argv[i+1];
            else cout << "Error: No csv filename given";
            i++;
        }
        else {
            cout << "Error: Invalid parameter\n";
            print_help_page();
            return -1;
        }
    }


    yyin = fopen(input_file.c_str(), "r");
    if (!yyin) {
        yyin = stdin;
    }

    // if(argc != 3) {
    //     cout << "Something went terribly wrong." << endl;
    //     exit(-1);
    // }
    // if (argc > 1) {
    //     yyin = fopen(argv[1], "r");
    // } else {
    //     yyin = stdin;
    // }
    // if(string(argv[2]) == "true") {
    //     // verbose has been passed
    //     verbose_flag = true;
    // }
    INDENT_STACK.push(0);
    OFFSET_STACK.push(-1); // -1 should always be at the bottom of the stack
    ST_STACK.push(nullptr); // nullptr will denote bottom of the stack
    LABEL_CNT_STACK.push(0);

    SYMBOL_TABLE = new symbol_table(GLOBAL, "GLOBAL", NULL);
    dummy_entry = new st_entry("__name__", D_STRING, OFFSET, -1, SYMBOL_TABLE->scope);
    SYMBOL_TABLE->add_entry(dummy_entry);
    OFFSET += dummy_entry->size;
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
    
    csvFile.open(sym_csv);
    csvFile << "name,b_type,size,offset,decl_line,scope\n";
    SYMBOL_TABLE->output_st_to_csv();
    csvFile.close();

    if(verbose_flag) {
        cout << "Symbol table created!" << endl;
    }

    AST_ROOT->generate_3ac();
    if(verbose_flag) {
        cout << "3AC generated!" << endl;
    }

    output_3AC_to_txt(tac_file);

    // SYMBOL_TABLE->print_st();

    Instruction_Wrapper* x86 = new Instruction_Wrapper(x86_filename);
    x86->gen_x86_init();
    x86->gen_x86(IR, SYMBOL_TABLE);

    output_x86(x86_filename, x86);  


    return 0;
}
