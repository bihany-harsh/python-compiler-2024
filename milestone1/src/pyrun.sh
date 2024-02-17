#!/bin/bash

bison -d pyparse.y
flex pylex.l
g++ lex.yy.c pyparse.tab.c -ll
./a.out $1

rm lex.yy.c pyparse.tab.c pyparse.tab.h a.out
