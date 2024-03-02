#!/bin/bash

bison -d pyparse.y
flex pylex.l
g++ --std=c++17 main.cpp node.cpp lex.yy.c pyparse.tab.c -ll
./a.out $1
dot -Tpdf graph.dot -o graph2.pdf

# rm lex.yy.c pyparse.tab.c pyparse.tab.h a.out
