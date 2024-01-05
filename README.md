# boolean-algebra-ast-visualiser

Visualise the Abstract Syntax Tree(AST) that's generated from a boolean expression.

## Build project (Linux)

To build the project, simply run the following command:
```bash
$ ./build.sh
```

You can also simultaneously run it via:
```bash
$ ./build.sh run
```

Once it's built, the result will be in the build directory, which will be created if it doesn't exist.

## How it works

When you run the program, it starts up a REPL. Here you can use the commands `help()` to print the help text and `quit()` to close the REPL. You can also type in a boolean expression to visualise it's AST.

Example session:

```bash
$ ./build/main
*** Boolean Algebra AST visualiser ***
The purpose of this program is to visualise the Abstract Syntax Tree (AST)
of boolean algebra expressions
Simply type an expression and the generated AST will be visualised
For example: "ab + c + a!d ^ b*c"
For more information type help()
To quit type quit()
> a + b + c
OR(
|   OR(
|   |   a
|   |   b
|   )
|   c
)
> a + !(b^c)
OR(
|   a
|   NOT(
|   |   XOR(
|   |   |   b
|   |   |   c
|   |   )
|   )
)
> quit()
```
