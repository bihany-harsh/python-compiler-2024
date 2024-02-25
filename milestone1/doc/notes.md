## Lines

* Any line can have a `explicit line joiner` anywhere within the line.
* Expressions in square brackets, parantheses and curly brackets may be split into more than one physical lines without using backslash (continued lines)
* Indentation of continued lines is not required.
* Indentation is rejected as inconsistent if a source file mixes tabs and spaces in a way that makes the meaning dependent on the worth of a tab in spaces; a `TabError` is raised in this case.
* We shall have only two spaces as for indentation. Hence any `\t` in the beginning of the line marking indentation is erroneous.
* *Stack used to maintain indentation level.*
* *Before the first line of the file is read, a single zero is pushed on the stack; this will never be popped off again. The numbers pushed on the stack will always be strictly increasing from bottom to top. **At the beginning of each logical line,** the line’s indentation level is compared to the top of the stack. If it is equal, nothing happens. If it is larger, it is pushed on the stack, and one INDENT token is generated. If it is smaller, it must be one of the numbers occurring on the stack; all numbers on the stack that are larger are popped off, and for each number popped off a DEDENT token is generated. At the end of the file, a DEDENT token is generated for each number remaining on the stack that is larger than zero.*
