# Project simplCT [EN]

This repository is the base for the simplCT compilation project.

## Team members
- Student 1: `Fabio` `DEVINE` (< fbdv94 >)
- Student 2: `Lina` `AGHA` (< GitHub username >)
- Group: `P1-INT1` (e.g. P1-INT)

*Keep only the "Team members" section, from here you can modify your README to document your project*

## Quick start
- A CMakeLists.txt is provided to allow immediate compilation in CLion or command line with CMake.

## Git best practices (working in pairs)
### 1. Work with small branches
- Create a branch per task: `feat/lexer-tokenize`, `fix/lexer-errors`, etc.
- Avoid doing everything directly on `main`.

### 2. Make small, readable commits
- One commit = one idea.
- Recommended message:
  - `feat: add tokenize loop skeleton`
  - `fix: handle unterminated string token`
  - `refactor: split scan_number helper`

### 3. Push often
- Always test and validate locally before pushing.
- Push your branch regularly to avoid losing work. 
- This also makes it easier for your partner to review.

### 4. Review before merging
- Open a Pull Request (PR) even in pairs.
- Review the modified files before validation.
- This helps catch mistakes and ensures both partners understand the changes.

### 5. Sync before continuing
- Before coding a new task: **always** pull the latest changes before starting.
- This avoids conflicts and ensures you are working on the most up-to-date base.

## Reminder
- The submitted code must remain your work.
- You must be able to explain every part of the code.
- Don't hesitate to ask for help from your teachers as soon as you are stuck (on code or on git)

### Implementation Summary
We built a lexer that reads the source code character by character using a `t_lexer` structure.
We used helper functions (`peek_char`, `peek_next_char`, `advance_pos`) to safely move through the text.
We created separate functions to recognize tokens:
* identifiers and keywords
* numbers
* strings
The `tokenize` function checks each character and calls the right function. It also handles operators and adds all tokens to a list.

### Work Distribution
We split the work evenly by doing one function out of two each.
For the final function (`tokenize`), we worked together to complete and test it.


Project Goals

The goal of the simplCT project is to develop a small compiler/interpreter for a simplified programming language inspired by C.
The project helps understand the different stages involved in how a programming language works: lexical analysis, bytecode compilation, and execution through a virtual machine.

In this part of the project, I implemented:
the lexer used to transform source code into tokens,
the compiler that generates bytecode,
the management of expressions and statements (let, print, if, while),
the execution of the program inside the virtual machine.

Architecture

The project is divided into several modules:
lexer.c: transforms the source code into a list of tokens.
token.c: manages token-related structures and functions.
compiler.c: parses tokens and generates bytecode instructions.
bytecode.c: contains the structures representing bytecode instructions.
vm.c: executes the generated instructions.
environment.c: manages variables and their storage.
io_utils.c: contains utility functions for input/output operations.

The global workflow is the following:

The lexer reads the source code.
The tokens are sent to the compiler.
The compiler generates bytecode.
The virtual machine executes the bytecode.
Work Distribution

I completed the project alone.

Use of AI

I used ChatGPT occasionally to help understand the origin of certain bugs and better interpret some compilation error messages and project behavior.

Known Bugs and Limitations
String management remains limited.
The simplCT language is intentionally minimal and does not support some advanced features found in real compilers.
