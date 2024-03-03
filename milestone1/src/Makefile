# Tools and compiler
CXX = g++
CXX_FLAGS = -std=c++17
LEX = flex
BISON = bison
DOT = dot
INPUT_FILE = ../testcases/public1.py

# Source and object files
LEX_SOURCE = pylex.l
BISON_SOURCE = pyparse.y
CPP_SOURCES = main.cpp node.cpp
HEADER_FILE = include/node.hpp
OBJECTS = lex.yy.o pyparse.tab.o $(CPP_SOURCES:.cpp=.o)

# Output files and graph
BINARY = a.out
GRAPH_PDF = graph.pdf
GRAPH_DOT = graph.dot

# Default target
all: $(GRAPH_PDF)

# generating the output PDF
$(GRAPH_PDF): $(GRAPH_DOT)
	$(DOT) -Tpdf $^ -o $@

$(GRAPH_DOT): $(BINARY) $(INPUT_FILE)
	./$(BINARY) $(INPUT_FILE)

$(BINARY): lex.yy.c pyparse.tab.c $(CPP_SOURCES)
	$(CPP) -o $@ $^ -ll

lex.yy.c: $(LEX_SOURCE) pyparse.tab.h
	$(LEX) $(LEX_SOURCE)

pyparse.tab.c pyparse.tab.h: $(BISON_SOURCE) $(HEADER_FILE)
	$(BISON) -d $(BISON_SOURCE)

# Clean target
clean:
	rm -f lex.yy.c pyparse.tab.c pyparse.tab.h $(BINARY) $(GRAPH_DOT) $(GRAPH_PDF) $(OBJECTS)
