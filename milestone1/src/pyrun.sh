#!/bin/bash

# This script automates the running of the makefile
# It supports the command line arguments -help, -h, -input, -i, -output, -o, -verbose, -v

input="../tests/public1.py"
output="graph.pdf"
verbose="false"

show_usage() {
    echo "Usage: ./pyrun.sh [-help]/[-h] [-input <file>]/[-i <file>] [-output <file>]/[-o <file>] [-verbose]/[-v]"
    echo "-input [-i] <file>, -output [-o] <file> specifies the input and output files respectively. Ensure that you provide the relative path."
    echo "-verbose [-v] prints additional checkpoints to show the progress during execution."
    echo "These options can be used in any order."
}

# Parse the command line arguments
while [ $# -gt 0 ]; do
  case "$1" in
    -help|-h)
      show_usage
      exit 0
      ;;
    -input|-i)
      if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
        input="$2"
        shift
      else
        echo "Error: $1 requires a file argument."
        echo ""
        show_usage
        exit 1
      fi
      ;;
    -output|-o)
      if [ -n "$2" ] && [ "${2:0:1}" != "-" ]; then
        output="$2"
        shift
      else
        echo "Error: $1 requires a file argument."
        echo ""
        show_usage
        exit 1
      fi
      ;;
    -verbose|-v)
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

make INPUT_FILE="$input" GRAPH_PDF="$output" VERBOSE="$verbose" 
