# HDS COMPILER

The following repository holds the code for a compiler that compiles basic Python scripts, as a course project for [CS335: Semester II (2024-25)](https://www.cse.iitk.ac.in/users/swarnendu/courses/spring2024-cs335/), under the guidance of [Prof. Swarnendu Biswas](https://www.cse.iitk.ac.in/users/swarnendu/). This compiler was made in a team of three which included me, along with [Siddhant Jakhotiya](https://github.com/ahahahahah1) and [Danish Mehmood](https://github.com/danx069).

### A near-to exhaustive list of the features supported by our compiler...

* Statically typed version of Python, supported. Basically C-like semantics in Python. View the examples in `./milestone3/tests/`.
* Integer (`int`) and string (`str`) data types.
* 1-D lists of uniform data types.
* All basic arithmetic, relational logical, bitwise and augmented assigment operators supported. Note that ternary operations are not supported.
* Basic language features like the `if-elif-else`, `while` and `for` - loops supported. Note that the for loop supported is only of the type:

```python
for i in range(a):
	# code
```

```python
for i in range(a, b):
	# code
```

* Recursive methods supported.
* Support the library function `print()` for only printing the primitive Python types, one at a time.
* Support of classes and objects with a `__init__` method, including multilevel inheritance and constructors, with their method calls supported and also access to members declared with a `self` directive.
* Static polymorphism via method overloading.
* The compiler requires the following block:

  ```python-repl
  if __name__ == "__main__":
  	# your function calls.
  ```
* Do note that format specifiers within print statements are not supported and neither are sequence of object receivers. `import` statements are not supported, no string operations except string comparison.
* Multiple function calls can be executed within a single line.
* Can return lists and class objects from functions and can also take them as arguments to function calls.
* Support for the `len(array)` within well-defined scopes.

---



### Requirements

Our compiler requires to have `flex`, `bison` installed along with the `gcc` compiler.

### Working...

`cd` into the milestone3 directory and:

```bash
make
```

This generates our `pycompiler.o`

```bash
./pycompiler.o -i ../tests/test1.py -o graph.dot -a asm1.s -s sym_tab.csv -t tac.txt -v
```

is the complete set of flags, supported (optionally run `./pycompiler -h` for more information). You can omit the `-o, -t, -s`, and `-v `flags. However, the `-a` flag is needed to generate the x86 code for the python script, which can be run using

```bash
./pyrun.sh asm1.s
```

You can optionally provide the .dot file (if generated using the -o from the pycompiler) to generate the AST (abstract symbol table) of the python script, in a .pdf file, much like:

```bash
./pyrun.sh asm1.s graph.dot 
```




The project is divided into 3 milestones, the first of which developed the frontend of the compiler, the lexer and the scanner. The second accomplished the making of the IR (intermediate representation) - 3AC code and the last milestone was based off around generating x86 code for the generated IR.

---



~*In small deeds do we live, in lesser we die...*
