// #include <iostream>
// #include <string>
// #include <vector>
// #include "include/x86.hpp"
// #include "include/_3AC.hpp"

// using namespace std;

// extern symbol_table* SYMBOL_TABLE;
// extern vector<Quadruple*> IR;

// extern void yyerror(const char*);

// int N_FUNC = 0;
// int string_counter = 0; // counts the number of strings in the data segment
// string CURR_FUNC_LABEL = "";
// bool rax_has_temp = false;
// // bool reg_has_temp[6] = {false};

// // vector <Instruction*> x86_code;

// bool is_variable(string s) {
//     return !(s[0] >= '0' && s[0] <= '9') && (s[0] != '-') && (s[0] != '+');
// }

// symbol_table_entry* get_func_entry(symbol_table* ST, string func_label) {
//     symbol_table* st = ST;
//     while(st) {
//         for(st_entry* entry: st->entries) {
//             if(entry->b_type == D_FUNCTION && entry->label == func_label) {
//                 return entry;
//             }
//         }
//         st = st->parent;
//     }

//     return nullptr; // could not find entry
// }

// void output_x86(const string& filename, Instruction_Wrapper* x86) {
//     ofstream txtFile(filename);
//     if (!txtFile.is_open()) {
//         std::cerr << "Failed to open file for writing.\n";
//         exit(1);
//     }
//     for (Instruction* instr: x86->data_segment) {
//         txtFile << instr->code << endl;
//     }
//     for (Instruction* instr: x86->text_segment) {
//         txtFile << instr->code << endl;
//     }
// }

// Instruction::Instruction(string op, string dst, string src1, string src2, instruction_type type) {
//     this->op = op;
//     this->dst = dst;
//     this->src1 = src1;
//     this->src2 = src2;
//     this->type = type;

//     this->code = this->make_code();   
// }

// string Instruction::make_code() {
//     string code = "";
//     switch(this->type) {
//         case I_LABEL:
//             code = this->op + ":";
//             break;
//         case I_SEGMENT:
//             code = "\t" + this->op;
//             break;
//         case I_DIRECTIVE:
//             code = "\t" + this->op;
//             if (this->dst != "") {
//                 code += " " + this->dst;
//                 if (this->src1 != "") {
//                     code += ", " + this->src1;
//                     if (this->src2 != "") {
//                         code += ", " + this->src2;
//                     }
//                 }
//             } else {
//                 code = "\t" + this->op;
//             }
//             break;
//         case I_INSTRUCTION:
//             code = "\t" + this->op;
//             if (this->dst != "") {
//                 code += " " + this->dst;
//                 if (this->src1 != "") {
//                     code += ", " + this->src1;
//                     if (this->src2 != "") {
//                         code += ", " + this->src2;
//                     }
//                 }
//             }
//         default:
//             break;
//     }
//     return code;
// }


// void Instruction_Wrapper::gen_x86_global_decl(Quadruple* quad, Segment_type &prev_seg_type, symbol_table* scope) {
//     Instruction* instr;
//     symbol_table_entry* st_entry;

//     instr = new Instruction(".globl", quad->result, "", "", I_DIRECTIVE);
//     this->text_segment.push_back(instr);

//     if (prev_seg_type == SEG_TEXT) {
//         // in data segment
//         instr = new Instruction(".data", "", "", "", I_SEGMENT);
//         this->text_segment.push_back(instr);
//     }
    
//     st_entry = scope->get_entry(quad->result);
//     prev_seg_type = SEG_DATA;
    
//     if (st_entry->b_type == D_INT) {
//         instr = new Instruction(".align", "4", "", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);
        
//         instr = new Instruction(".type", quad->result, "@object", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".size", quad->result, "4", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(quad->result, "" , "", "", I_LABEL);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".long", quad->arg1, "", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);
//     } else if (st_entry->b_type == D_BOOL) {
//         instr = new Instruction(".type", quad->result, "@object", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".size", quad->result, "1", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(quad->result, "" , "", "", I_LABEL);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".byte", quad->arg1 == "True" ? "1" : "0", "", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);
//     } else if (st_entry->b_type == D_STRING) {
//         int size = quad->arg1.size() - 3; // removing the \" and adding space for \0
//         // quad->arg1 is of the form \"abc\" 
//         string str = "\"" + quad->arg1.substr(2, quad->arg1.size() - 4) + "\"";
//         if (size >= 8) {
//             instr = new Instruction(".align", to_string((size > 1) ? 1 << ((sizeof(size) << 3) - __builtin_clz(size - 1)) : 0), "", "", I_DIRECTIVE);
//             this->text_segment.push_back(instr);
//         }
//         instr = new Instruction(".type", quad->result, "@object", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".size", quad->result, to_string(size), "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(quad->result, "", "", "", I_LABEL);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".string", str, "", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);
//     }
// }

// void Instruction_Wrapper::gen_x86_func_def(Quadruple* quad, Segment_type &prev_seg_type, symbol_table** scope) {
//     Instruction* instr;
//     symbol_table_entry* st_entry_func;

//     if (quad->arg1 == "begin_func") {
//         if (prev_seg_type == SEG_DATA) {
//             instr = new Instruction(".text", "", "", "", I_SEGMENT);
//             this->text_segment.push_back(instr);
//         }

//         CURR_FUNC_LABEL = quad->arg2;

//         instr = new Instruction(".globl", quad->arg2, "", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".type", quad->arg2, "@function", "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(quad->arg2, "", "", "", I_LABEL);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".LFB" + to_string(N_FUNC), "", "", "", I_LABEL);
//         this->text_segment.push_back(instr);

//         // push rbp
//         instr = new Instruction("pushq", "%rbp", "", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);

//         // mov rsp to rbp
//         instr = new Instruction("movq", "%rsp", "%rbp", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);

//         st_entry_func = get_func_entry(*scope, quad->arg2);
//         // cout << "func_size: " << st_entry_func->size << endl;
//         *scope = st_entry_func->child_symbol_table;
//         this->update_func_offsets(*scope);  
//     } else if (quad->arg1 == "end_func") {
//         // pop rbp
//         instr = new Instruction("popq", "%rbp", "", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);

//         instr = new Instruction ("ret", "", "", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".LFE" + to_string(N_FUNC++), "", "", "", I_LABEL);
//         this->text_segment.push_back(instr);

//         instr = new Instruction(".size", quad->arg2, ".-" + quad->arg2, "", I_DIRECTIVE);
//         this->text_segment.push_back(instr);

//         *scope = (*scope)->parent;

//         CURR_FUNC_LABEL = "";
//         rax_has_temp = false;
//     }
// }

// void Instruction_Wrapper::gen_x86_jump(Quadruple* quad, symbol_table* scope) {
//     Instruction* instr;
//     base_data_type return_type;
    
//     if (quad->op == "return") {
//         return_type = get_func_entry(scope, CURR_FUNC_LABEL)->f_attr.return_type;
//         if (return_type == D_INT) {           
//             if (scope->get_entry(quad->result)) { // say `return x + y` {return t1 (x+y)} (then rax would already have the result)
//                 instr = new Instruction("movq", to_string(scope->get_entry(quad->result)->offset) + "(%rbp)", "%rax", "", I_INSTRUCTION);
//                 this->text_segment.push_back(instr);
//             } else {
//                 // return 0
//                 instr = new Instruction("movq", "$" + quad->result, "%rax", "", I_INSTRUCTION);
//                 this->text_segment.push_back(instr);
//             }
//         } else if (return_type == D_BOOL) {
//             instr = new Instruction("movb", quad->result, "%al", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//     }
// }

// void Instruction_Wrapper::gen_x86_print(Quadruple* quad, symbol_table* scope) {
//     Instruction* instr;
//     symbol_table_entry* st_entry;

//     // because it is a function call
//     instr = new Instruction("subq", "$16", "%rsp", "", I_INSTRUCTION);
//     this->text_segment.push_back(instr);

//     if(quad->arg1[0] == '\\') {
//         /*
//             print \"hello world
//         */
//         // adding the string constant to the data segment
//         instr = new Instruction(".str" + to_string(string_counter++), "", "", "", I_LABEL);
//         this->data_segment.push_back(instr);
//         instr = new Instruction(".string", "\"" + quad->arg1.substr(2, quad->arg1.length() - 4) + "\"", "", "", I_DIRECTIVE);
//         this->data_segment.push_back(instr);
        
//         // making the operations for printing the string
//         instr = new Instruction("leaq", ".str" + to_string(string_counter - 1) + "(%rip)", "%rdi", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);
//         instr = new Instruction("movq", "$0", "%rax", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);
//         instr = new Instruction("call", "printf@PLT", "", "", I_INSTRUCTION);
//         this->text_segment.push_back(instr);
//     }
//     else {
//         /* 
//             print x : where x is a string variable
//             print x : where x is a bool/int
//             print x : where x is a 3AC temporary
//             print 1 : where 1 is a constant
//         */
//         if (is_variable(quad->arg1)) {
//             if (scope->get_entry(quad->arg1)) {
//                 st_entry = scope->get_entry(quad->arg1);
//                 if (st_entry->b_type == D_STRING) {
//                     instr = new Instruction("movq", to_string(st_entry->offset) + "(%rbp)", "%rdi", "", I_INSTRUCTION);
//                     this->text_segment.push_back(instr);
//                     instr = new Instruction("call", "puts@PLT", "", "", I_INSTRUCTION);
//                     this->text_segment.push_back(instr);
//                 } else if (st_entry->b_type == D_BOOL || st_entry->b_type == D_INT) {
//                     instr = new Instruction("movq", to_string(st_entry->offset) + "(%rbp)", "%rsi", "", I_INSTRUCTION);
//                     this->text_segment.push_back(instr);
//                     instr = new Instruction("leaq", ".str0(%rip)", "%rdi", "", I_INSTRUCTION);
//                     this->text_segment.push_back(instr);
//                     instr = new Instruction("movq", "$0", "%rax", "", I_INSTRUCTION);
//                     this->text_segment.push_back(instr);
//                     instr = new Instruction("call", "printf@PLT", "", "", I_INSTRUCTION);
//                     this->text_segment.push_back(instr);
//                 }
//             } else {
//                 // somthing like print(f(3, 4)) => print t1
//                 instr = new Instruction("movq", "%rax", "%rsi", "", I_INSTRUCTION);
//                 this->text_segment.push_back(instr);
//                 instr = new Instruction("leaq", ".str0(%rip)", "%rdi", "", I_INSTRUCTION);
//                 this->text_segment.push_back(instr);
//                 instr = new Instruction("movq", "$0", "%rax", "", I_INSTRUCTION);
//                 this->text_segment.push_back(instr);
//                 instr = new Instruction("call", "printf@PLT", "", "", I_INSTRUCTION);
//                 this->text_segment.push_back(instr);
//             }
//         } else {
//             // print 4

//             instr = new Instruction("movq", "$" + quad->arg1, "%rsi", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);

//             instr = new Instruction("leaq", ".str0(%rip)", "%rdi", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);

//             instr = new Instruction("movq", "$0", "%rax", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);

//             instr = new Instruction("call", "printf@PLT", "", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);           

//             // if (quad->arg1[0] == '0' || quad->arg1[0] == '1') {
//             //     instr = new Instruction("movq", "$" + quad->arg1, "%rdi", "", I_INSTRUCTION);
//             //     this->text_segment.push_back(instr);
//             //     instr = new Instruction("movq", "$0", "%rax", "", I_INSTRUCTION);
//             //     this->text_segment.push_back(instr);
//             //     instr = new Instruction("call", "printf@PLT", "", "", I_INSTRUCTION);
//             //     this->text_segment.push_back(instr);
//             // }
//         }
//     }
// }

// void Instruction_Wrapper::update_func_offsets(symbol_table* scope) {
//     st_entry* st_entry_func = get_func_entry(scope->parent, CURR_FUNC_LABEL);

//     int num_args = st_entry_func->f_attr.num_args;
//     int num_local_vars = scope->entries.size() - num_args;

//     int stack_pos = -(st_entry_func->size - (num_local_vars)*8);
//     bool direction = false; // false means -8
//     for (int i = 0; i < scope->entries.size(); i++) {
//         if (num_args > 6) {
//             if (i == num_args) {
//                 stack_pos = -8;
//                 direction = false;
//             }
//             if (i == 6) {
//                 stack_pos = 16;
//                 direction = true;
//             }
//             scope->entries[i]->offset = stack_pos;
//             if (direction) {
//                 stack_pos += 8;
//             } else {
//                 stack_pos -= 8;
//             }
//         } else {
//             if (i == num_args) {
//                 stack_pos = -8;
//             } 
//             scope->entries[i]->offset = stack_pos;
//             stack_pos -= 8;
//         }
//     }
// }

// void Instruction_Wrapper::gen_x86_func_arg(vector<Quadruple*> pop_IR, symbol_table* scope) {
//     Instruction* instr;

//     // this->update_func_offsets(scope);

//     // now the movq %arg -offset(%rbp)
//     for (int i = 0; ((i < pop_IR.size()) && (i < 6)); i++) {
//         if (i == 0) {
//             instr = new Instruction("movq", "%rdi", to_string(scope->get_entry(pop_IR[i]->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//         if (i == 1) {
//             instr = new Instruction("movq", "%rsi", to_string(scope->get_entry(pop_IR[i]->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//         if (i == 2) {
//             instr = new Instruction("movq", "%rdx", to_string(scope->get_entry(pop_IR[i]->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//         if (i == 3) {
//             instr = new Instruction("movq", "%rcx", to_string(scope->get_entry(pop_IR[i]->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//         if (i == 4) {
//             instr = new Instruction("movq", "%r8", to_string(scope->get_entry(pop_IR[i]->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//         if (i == 5) {
//             instr = new Instruction("movq", "%r9", to_string(scope->get_entry(pop_IR[i]->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//             this->text_segment.push_back(instr);
//         }
//     }
// }

// void Instruction_Wrapper::gen_x86_func_call(vector<Quadruple*> push_IR, symbol_table* scope) {
//     Instruction* instr;
//     symbol_table_entry* st_entry;
//     int num_args = push_IR.size();
//     int num_local_vars = scope->entries.size() - num_args;

//     // this->update_func_offsets(scope);
// }

// void Instruction_Wrapper::gen_x86_binary(Quadruple* quad, symbol_table* scope) {
//     Instruction* instr;

//     /*
//         t1 = a + b
//         t1 = t2 + a
//         t1 = t2 + t3
//         t1 = a + t2
//         t1 = a + 4
//         t1 = t2 + 4
//         t1 = 4 + 5
//         t1 = 4 + a
//         t1 = 4 + t2
//     */

//     if (scope->get_entry(quad->result)) {
//         // an entry inside is being assigned
//         // should not happen!
//         yyerror("x86 gen: Should not have had happened");
//     } else {
//         // a 3AC temporary is being assigned
//         if (is_variable(quad->arg1)) {
//             /*
//                 t1 = a + b
//                 t1 = t2 + a
//                 t1 = t2 + t3
//                 t1 = a + t2
//                 t1 = a + 4
//                 t1 = t2 + 4
//             */
//             if (scope->get_entry(quad->arg1)) {
//                 /*
//                     t1 = a + b
//                     t1 = a + t2
//                     t1 = a + 4
//                 */
//                 if (is_variable(quad->arg2)) {
//                     /*
//                         t1 = a + b
//                         t1 = a + t2
//                     */

//                     if (scope->get_entry(quad->arg2)) {
//                         /*
//                             t1 = a + b
//                         */
//                         if (rax_has_temp) {
//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg2)->offset) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);    

//                             if (quad->op == "+") {
//                                 instr = new Instruction("addq", "%rcx", "%rdx", "", I_INSTRUCTION);
//                                 this->text_segment.push_back(instr);
//                             } // TODO: add all other operator support
//                         } else {
//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg2)->offset) + "(%rbp)", "%rax", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);    

//                             if (quad->op == "+") {
//                                 instr = new Instruction("addq", "%rdx", "%rax", "", I_INSTRUCTION);
//                                 this->text_segment.push_back(instr);
//                             } // TODO: add all other operator support
//                         }                          
//                     } else {
//                         /*
//                             t1 = a + t2
//                         */

//                         cout << quad->code << endl;

//                         if (rax_has_temp) {
//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             instr = new Instruction("leaq", "(%rax)", "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             if (quad->op == "+") {
//                                 instr = new Instruction("addq", "%rcx", "%rdx", "", I_INSTRUCTION);
//                                 this->text_segment.push_back(instr);
//                             } // TODO: add all other operator support

//                             instr = new Instruction("leaq", "(%rdx)", "%rax", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         } else {
//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             if (quad->op == "+") {
//                                 instr = new Instruction("addq", "%rdx", "%rax", "", I_INSTRUCTION);
//                                 this->text_segment.push_back(instr);
//                             } // TODO: add all other operator support
//                         }
//                     }
//                 } else {
//                     /*
//                         t1 = a + 4
//                     */

//                     if (rax_has_temp) {
//                         instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);

//                         if (quad->op == "+") {
//                             instr = new Instruction("addq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         }
//                     } else {
//                         instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", "%rax", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);

//                         if (quad->op == "+") {
//                             instr = new Instruction("addq", "$" + quad->arg2, "%rax", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         }
//                     }                    
//                 }
//             } else {
//                 /*
//                     t1 = t2 + a
//                     t1 = t2 + t3
//                     t1 = t2 + 4
//                 */
//                 if (is_variable(quad->arg2)) {
//                     /*
//                         t1 = t2 + a
//                         t1 = t2 + t3
//                     */
//                     if (scope->get_entry(quad->arg2)) {
//                         /*
//                             t1 = t2 + a
//                         */

//                         if (rax_has_temp) {
//                             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg2)->offset) + "(%rbp)", "%rcx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             instr = new Instruction("leaq", "(%rax)", "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);

//                             if (quad->op == "+") {
//                                 instr = new Instruction("addq", "%rcx", "%rdx", "", I_INSTRUCTION);
//                                 this->text_segment.push_back(instr);
//                             } // TODO: add all other operator support

//                             instr = new Instruction("leaq", "(%rdx)", "%rax", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         } else {
//                             // should not happen!
//                             yyerror("x86 gen: Should not have had happened");
//                         }
//                     } else {
//                         /*
//                             t1 = t2 + t3
//                         */

//                         if (rax_has_temp) {
//                             if (quad->op == "+") {
//                                 instr = new Instruction("addq", "%rax", "%rdx", "", I_INSTRUCTION);
//                                 this->text_segment.push_back(instr);
//                             }

//                             instr = new Instruction("leaq", "(%rdx)", "%rax", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         } else {
//                             // should not happen!
//                             yyerror("x86 gen: Should not have had happened");
//                         }
//                     }
//                 } else {
//                     /*
//                         t1 = t2 + 4
//                     */

//                     if (rax_has_temp) {
//                         if (quad->op == "+") {
//                             instr = new Instruction("addq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         }
//                         instr = new Instruction("leaq", "(%rdx)", "%rax", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);
//                     } else {
//                         // should not happen!
//                         yyerror("x86 gen: Should not have had happened");
//                     }
//                 }
//             }
//         } else {
//             /*
//                 t1 = 4 + 5
//                 t1 = 4 + a
//                 t1 = 4 + t2
//             */
//             if (is_variable(quad->arg2)) {
//                 /*
//                     t1 = 4 + a
//                     t1 = 4 + t2
//                 */
//                if (scope->get_entry(quad->arg2)) {
//                     /*
//                         t1 = 4 + a
//                     */

//                     if (rax_has_temp) {
//                         instr = new Instruction("movq", to_string(scope->get_entry(quad->arg2)->offset) + "(%rbp)", "%rdx", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);

//                         if (quad->op == "+") {
//                             instr = new Instruction("addq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         }
//                     } else {
//                         instr = new Instruction("movq", to_string(scope->get_entry(quad->arg2)->offset) + "(%rbp)", "%rax", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);

//                         if (quad->op == "+") {
//                             instr = new Instruction("addq", "$" + quad->arg1, "%rax", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         }
//                     }
//                } else {
//                     /*
//                         t1 = 4 + t2
//                     */
//                     if (rax_has_temp) {
//                         if (quad->op == "+") {
//                             instr = new Instruction("addq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
//                             this->text_segment.push_back(instr);
//                         }
//                         instr = new Instruction("leaq", "(%rdx)", "%rax", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);
//                     } else {
//                         // should not have happened
//                         yyerror("x86 gen: Should not have had happened");
//                     }
//                }
//             } else {
//                 /*
//                    t1 = 4 + 5 
//                 */
//                 if (rax_has_temp) {
//                     if (quad->op == "+") {
//                         instr = new Instruction("movq", "$" + quad->arg1, "%rdx", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);

//                         instr = new Instruction("addq", "$" + quad->arg2, "%rdx", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);
//                     }
//                 } else {
//                     if (quad->op == "+") {
//                         instr = new Instruction("movq", "$" + quad->arg1, "%rax", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);

//                         instr = new Instruction("addq", "$" + quad->arg2, "%rax", "", I_INSTRUCTION);
//                         this->text_segment.push_back(instr);
//                     }
//                 }
//             }
//         }

//         rax_has_temp = true;
//         // %rax has a temporary
//     }
// } 

// void Instruction_Wrapper::gen_x86_assign(Quadruple* quad, symbol_table* scope) {
//     /*
//         a = t2
//         a = 4
//         a = b
//     */
//     Instruction* instr;
//     if (is_variable(quad->arg1)) {
//         // if (rax_has_temp) {
//         //     instr = new Instruction("leaq", "(%rdx)", "%rax", "", I_INSTRUCTION);
//         //     this->text_segment.push_back(instr);
//         // }
//         if (scope->get_entry(quad->arg1)) {
//             instr = new Instruction("movq", to_string(scope->get_entry(quad->arg1)->offset) + "(%rbp)", to_string(scope->get_entry(quad->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//         } else {
//             instr = new Instruction("movq", "%rax", to_string(scope->get_entry(quad->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//         }
//         rax_has_temp = false;
//     } else {
//         instr = new Instruction("movq", "$" + quad->arg1, to_string(scope->get_entry(quad->result)->offset) + "(%rbp)", "", I_INSTRUCTION);
//     }

//     this->text_segment.push_back(instr);
// }

// void Instruction_Wrapper::gen_x86(vector<Quadruple*> IR, symbol_table* SYMBOL_TABLE) {
//     symbol_table* scope = SYMBOL_TABLE;
//     string code = "";
//     Instruction* instr;
//     symbol_table_entry* st_entry;
//     int i = 0;
//     vector<Quadruple*> aux_IR;
//     Segment_type prev_seg_type = SEG_TEXT;

//     // for (; i < IR.size(); i++) {
//         // cout << IR[i]->code << "\t type is: " << IR[i]->q_type << endl;
//     //     if (IR[i]->q_type == Q_ASSIGN) {
//     //         // cout << "quad->result: " << IR[i]->result << " offset " << scope->get_entry(IR[i]->result)->offset <<endl;
//     //         if (scope->st_type ==  GLOBAL) {
//     //             this->gen_x86_global_decl(IR[i], prev_seg_type, scope);
//     //             prev_seg_type = SEG_DATA;              
//     //         }
//     //         else  {
//     //             this->gen_x86_assign(IR[i], scope);
//     //         }
//     //     } else if (IR[i]->q_type == Q_FUNC_LABEL) {
//     //         this->gen_x86_func_def(IR[i], prev_seg_type, &scope);
//     //     } else if (IR[i]->q_type == Q_JUMP) {
//     //         this->gen_x86_jump(IR[i], scope);
//     //     } else if (IR[i]->q_type == Q_POP_PARAM) {
//     //         aux_IR.clear();
//     //         while (IR[i]->q_type == Q_POP_PARAM) {
//     //             // cout << "quad->result: " << IR[i]->result << " offset " << scope->get_entry(IR[i]->result)->offset <<endl;
//     //             aux_IR.push_back(IR[i]);
//     //             i++;
//     //         }
//     //         i--;
//     //         this->gen_x86_func_arg(aux_IR, scope);
//     //     } else if (IR[i]->q_type == Q_BINARY) {
//     //         this->gen_x86_binary(IR[i], scope);
//     //     } else if (IR[i]->q_type == Q_PRINT) {
//     //         this->gen_x86_print(IR[i], scope);
//     //     } else if (IR[i]->q_type == Q_PUSH_PARAM) {
//     //         aux_IR.clear();
//     //         while (IR[i]->q_type == Q_PUSH_PARAM) {
//     //             // cout << "quad->result: " << IR[i]->result << " offset " << scope->get_entry(IR[i]->result)->offset <<endl;
//     //             aux_IR.push_back(IR[i]);
//     //             i++;
//     //         }
//     //         i--;
//     //         // this->gen_x86_func_call(aux_IR, scope);
//     //     }
//     // }
// }

// Instruction_Wrapper::Instruction_Wrapper(string filename) {
//     this->filename = filename;
// }

// void Instruction_Wrapper::x86_init() {
//     Instruction* instr = new Instruction(".text", "", "", "", I_SEGMENT);
//     this->data_segment.push_back(instr);
//     instr = new Instruction(".section", ".rodata", "", "", I_DIRECTIVE);
//     this->data_segment.push_back(instr);
//     instr = new Instruction(".str" + to_string(string_counter++), "", "", "", I_LABEL);
//     this->data_segment.push_back(instr);
//     instr = new Instruction(".string", "\"%ld\\n\"", "", "", I_DIRECTIVE);
//     this->data_segment.push_back(instr);

//     // from this point on, we will be generating the text segment
//     instr = new Instruction(".text", "", "", "", I_DIRECTIVE);
//     this->text_segment.push_back(instr);
// }

