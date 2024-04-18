#!/bin/bash

# Check if at least one argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <x86_code.s> [graph.dot]"
    exit 1
fi

# Extract the assembly file name without extension
base_name="${1%.*}"

# Compile the assembly code to an object file
gcc -c "$1" -o "${base_name}.o"
if [ $? -ne 0 ]; then
    exit 2
fi

# Link the object file to create an executable
gcc "${base_name}.o" -o "${base_name}.out"
if [ $? -ne 0 ]; then
    exit 3
fi

# Run the executable
"./${base_name}.out"
if [ $? -ne 0 ]; then
    exit 4
fi

# Check if the second argument (dot file) is provided
if [ $# -gt 1 ]; then
    # Assume the second argument is the Graphviz dot file
    dot_file="$2"
    pdf_output="${dot_file%.*}.pdf"

    # Generate PDF from dot file
    dot -Tpdf "$dot_file" -o "$pdf_output"
    if [ $? -ne 0 ]; then
        exit 5
    fi
fi

exit 0
