#!/bin/bash

# This script automates the running of the makefile
# It supports the command line arguments -help, -input, -output, -verbose

input="../testcases/public1.py"
output="graph.pdf"
verbose="false"

show_usage() {
    echo "Usage: ./pyrun.sh [-help] [-input <file>] [-output <file>] [-verbose]"
    echo "-input <file> specifies the input file. Ensure that you provide the relative path of the python script."
    echo "-output <file> specifies the output PDF file depicting the AST of the given input program."
    echo "-verbose prints additional checkpoints to show the progress during execution."
    echo "These options can be used in any order."
}

# Parse the command line arguments
while [ $# -gt 0 ]; do
  case "$1" in
    -help)
      show_usage
      exit 0
      ;;
    -input)
      if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
        input="$2"
        shift
      else
        echo "Error: -input requires a file argument."
        echo ""
        show_usage
        exit 1
      fi
      ;;
    -output)
      if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
        output="$2"
        shift
      else
        echo "Error: -output requires a file argument."
        echo ""
        show_usage
        exit 1
      fi
      ;;
    -verbose)
      verbose="true"
      ;;
    *)
      echo "Error: Unsupported option $1."
      echo ""
      show_usage
      exit 1
      ;;
  esac
  shift
done

make INPUT_FILE=$input GRAPH_PDF=$output VERBOSE=$verbose 2>/dev/null