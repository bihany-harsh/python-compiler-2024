## Lines

* Any line can have a `explicit line joiner` anywhere within the line.
* Expressions in square brackets, parantheses and curly brackets may be split into more than one physical lines without using backslash (continued lines)
* Indentation of continued lines is not required.
* Indentation is rejected as inconsistent if a source file mixes tabs and spaces in a way that makes the meaning dependent on the worth of a tab in spaces; a `TabError` is raised in this case.
* We shall have only two spaces as for indentation. Hence any `\t` in the beginning of the line marking indentation is erroneous.
* *Stack used to maintain indentation level.*
