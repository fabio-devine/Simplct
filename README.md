# simplCT — A Mini Compiler and Virtual Machine in C

**simplCT** compiles programs written in a small language (`.simpl`) into bytecode and runs them on a stack-based virtual machine. It shows the whole pipeline, from source code to execution, and prints the tokens, the bytecode and the result at each stage.

```
Source (.simpl)  →  Lexer  →  Compiler  →  Bytecode  →  Virtual Machine  →  Output
```

## Project Context

Developed individually as a university project (designed for pairs), from a course template that provided the bytecode, the variable environment, the virtual machine and the tests.

**Developed by Fabio Devine:** the lexer, the compiler (expressions and statements), the main program, and the demo programs (GCD, prime numbers).

## The `.simpl` language

Variables, arithmetic (`+ - * /`), comparisons (`== != < > <= >=`), `print`, `if` and `while`:

```
let x = 10;
let y = 5;
let result = x + y;
print(result);
```

Expected output: `15`

## Build and run

Requires a C11 compiler (GCC or Clang), CMake 3.24+ and Git.

```bash
git clone https://github.com/fabio-devine/Simplct.git
cd Simplct
cmake -S . -B build
cmake --build build
./build/simplct programs/demo_pgcd.simpl
```

The program takes exactly one argument: the path to a `.simpl` file. Demo programs are in `programs/` (`demo_pgcd.simpl`, `demo_prime.simpl`, …).

## Tests

Lexer and compiler validation tests (CMake + CTest):

```bash
cmake -S . -B build-tests -DENABLE_VALIDATION_TESTS=ON
cmake --build build-tests
ctest --test-dir build-tests --output-on-failure
```

## Limitations

Educational project: small set of statements and expressions, limited string handling, simpler error handling than a production compiler.

## Technologies

C (C11), CMake, CTest, Git and GitHub, CLion.

## Use of AI

AI tools were used only as a learning aid: to understand concepts and to get help with bugs or dead ends (for example, understanding the origin of a bug or interpreting a compilation error). The lexer and compiler were written by the author.

## Author

**Fabio Devine**, developed individually as part of a university programming project.
