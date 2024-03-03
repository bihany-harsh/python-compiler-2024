# HDS COMPILER

A compiler made as a course project for CS335: Compiler Design under the guidance of Prof. Swarnendu Biswas in IIT Kanpur CSE Dept. in semester 2023-2024 II.

Group Members: Harsh Bihany (210406) Danish Mehmood (210297) Siddhant Suresh Jakhotiya (211030)

## Milestone 1

### Running the files

The implementation of Parser and Lexer is in Flex+Bison, and is written in C++. Further, DOT is used to create a graphical representation of the AST and exporting it to a PDF file. Users should install all the above (if not already installed) by running the following commands:

```bash
$ sudo apt-get update
$ sudo apt-get install flex
$ sudo apt-get install bison
$ sudo apt-get install graphviz
```

A script file `pyrun.sh` has been made to help run the program against a test case. The command line options provided by our program are as follows:

- `-i` or `-input`: Filename from which the compiler reads the Python program.
- `-o` or `-output`: Filename to which the compiler outputs the `DOT` file.
- `-v` or `-verbose`: Prints additional checkpoints to show progress.
- `-h` or `-help`: Shows the usage for the script.
- `-c` or `-clean`: Performs `make`, with given options (if any) and then performs `make clean`.

Each of the commands mentioned above is optional and can be utilized in any sequence. The script takes `../tests/test1.py` as default input and `graph.pdf` as the default output and doesn't print additional messages and does not perform `make clean` after execution

An example of a successful default execution is as follows:

```bash
$ ./pyrun.sh 
```

An example of successful execution using the command line options provided above against the test case `test3.py`, the output being redirected to `graph1.pdf` and with additional debug messages, is as follows:


```bash
$ ./pyrun.sh -input ../tests/test3.py -o graph1.pdf -verbose -c
```

---

