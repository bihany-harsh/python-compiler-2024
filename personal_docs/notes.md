
## Lines

* Any line can have a `explicit line joiner` anywhere within the line.
* Expressions in square brackets, parantheses and curly brackets may be split into more than one physical lines without using backslash (continued lines)
* Indentation of continued lines is not required.
* Indentation is rejected as inconsistent if a source file mixes tabs and spaces in a way that makes the meaning dependent on the worth of a tab in spaces; a `TabError` is raised in this case.
* We shall have only two spaces as for indentation. Hence any `\t` in the beginning of the line marking indentation is erroneous.
* *Stack used to maintain indentation level.*
* *Before the first line of the file is read, a single zero is pushed on the stack; this will never be popped off again. The numbers pushed on the stack will always be strictly increasing from bottom to top. **At the beginning of each logical line,** the line’s indentation level is compared to the top of the stack. If it is equal, nothing happens. If it is larger, it is pushed on the stack, and one INDENT token is generated. If it is smaller, it must be one of the numbers occurring on the stack; all numbers on the stack that are larger are popped off, and for each number popped off a DEDENT token is generated. At the end of the file, a DEDENT token is generated for each number remaining on the stack that is larger than zero.*
* For cleaning the tree, make changes in pyparse.y itself. The main focus is to remove non-terminals that were introduced by us.

  ```
  multiple_lines              :   multiple_lines single_line {
                                      $$ = new node(MULTIPLE_LINES, "MULTIPLE_LINES", false, NULL);
                                      if($1) {
                                          for(auto child: $1->children) {
                                              $$->add_parent_child_relation(child);
                                          }
                                      delete $1; //TODO: verify if correct
                                      }
                                      delete $1; //TODO: verify if correct
                                      $$->add_parent_child_relation($2);
                              }
                              |   {   $$ = NULL;  }
                              ;
  ```
  This removes unnecessary children from multiple_lines, adds them directly to the children of \$\$ and frees up the memory of \$1. Then finally it is connected to the FILE_INPUT terminal, making the entire program emerge from that start non-terminal. Also ensure that the order of appearance is retained. For example, if the children of the 2nd non-terminal are to be added to the children of the head of the production, first establish parent-child relationship between \$\$ and \$1 then iterate over the children of \$2.
* Also, instead of post-processing the tree (for example, to delete delimiters), instead of using helper functions, simply ignore them in the parser itself - refer to the production of *many_small_stmts*.
* Similarly, for tokens like *optional_semicolon*, simply don't access their tree_node so that it doesn't result in any segmentation errors.
* However, try to retain non-terminals that will make the process of 3AC generation easy. For example, arith_expr is term followed by many_arith_term. This format allows for 3AC code generation (since we can perform a simple *op* btw 2 operands in many_arith_expr). If we merge these non-terminals, that might make 3AC generation harder.
