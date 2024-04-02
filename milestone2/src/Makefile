# Tools and compiler
CXX = g++
CXX_FLAGS = -std=c++17
LEX = flex
BISON = bison
DOT = dot

# Source and object files
LEX_SOURCE = pylex.l
BISON_SOURCE = pyparse.y
CXX_SOURCES = main.cpp node.cpp symbol_table.cpp _3AC.cpp
HEADER_FILE = include/node.hpp

# Input, generated files and graph
INPUT_FILE = ../tests/test1.py
VERBOSE = false
BINARY = a.out
GRAPH_PDF = graph.pdf
GRAPH_DOT = graph.dot

# Default target
all: $(GRAPH_PDF)

verbose-flag : FORCE
	@echo $(VERBOSE) | cmp -s - $@ || echo $(VERBOSE > $@)
graph-file-name: FORCE
	@echo $(GRAPH_PDF) | cmp -s - $@ || echo $(GRAPH_PDF) > $@
input-file-name: FORCE
	@echo $(INPUT_FILE) | cmp -s - $@ || echo $(INPUT_FILE) > $@
FORCE:

# generating the output PDF
$(GRAPH_DOT): $(BINARY) input-file-name verbose-flag
	@rm input-file-name 
	@(./$(BINARY) $(INPUT_FILE) $(VERBOSE) || echo "Error in executing the program. Cannot generate dot script. \nExiting...")

$(GRAPH_PDF): $(GRAPH_DOT) input-file-name graph-file-name
	@rm graph-file-name
	@($(DOT) -Tpdf $(GRAPH_DOT) -o $(GRAPH_PDF))

$(BINARY): lex.yy.c pyparse.tab.c $(CXX_SOURCES)
	@$(CXX) $(CXX_FLAGS) $^ -ll

lex.yy.c: $(LEX_SOURCE) pyparse.tab.h
	@$(LEX) $(LEX_SOURCE)

pyparse.tab.c pyparse.tab.h: $(BISON_SOURCE) $(HEADER_FILE)
	@$(BISON) -d $(BISON_SOURCE)
# Clean target
clean:
	@rm -f lex.yy.c pyparse.tab.c pyparse.tab.h $(BINARY) input-file-name graph-file-name