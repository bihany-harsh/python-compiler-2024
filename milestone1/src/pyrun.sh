#!/bin/bash

# This script automates the running of the makefile
# It supports the command line arguments -help, -input, -output, -verbose

# Set the default values for the command line arguments
input="input.txt"
output="output.txt"
verbose="false"

# Parse the command line arguments
# TODO: Improve this segment of the code
while [ $# -gt 0 ]; do
  if [ "$1" == "-help" ]; then
    echo "Usage: pyrun.sh [-help] [-input <file>] [-output <file>] [-verbose -input <file>]"
    exit 0
  elif [ "$1" == "-input" ]; then
    input="$2"
    shift
  elif [ "$1" == "-output" ]; then
    output="$2"
    shift
  elif [ "$1" == "-verbose" ]; then
    verbose="true"
  fi
  shift
done

# Run the makefile
make input=$input output=$output verbose=$verbose
```