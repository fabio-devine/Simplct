#include <stdio.h>
#include <stdlib.h>

#include "headers/io_utils.h"
#include "headers/lexer.h"
#include "headers/token.h"
#include "headers/compiler.h"
#include "headers/bytecode.h"
#include "headers/vm.h"

int main(int argc, char** argv) {

    // =========================
    // CHECK ARGUMENTS
    // =========================

    if (argc != 2) {
        printf("Usage: %s <file.simpl>\n", argv[0]);
        return 1;
    }

    // =========================
    // READ SOURCE FILE
    // =========================

    char* source = read_file(argv[1]);

    if (!source) {
        fprintf(stderr, "Error: Could not read file %s\n", argv[1]);
        return 1;
    }

    // =========================
    // DISPLAY SOURCE CODE
    // =========================

    printf("=== SOURCE CODE ===\n");
    printf("%s\n", source);

    // =========================
    // LEXICAL ANALYSIS
    // =========================

    t_token_list* tokens = tokenize(source);

    if (!tokens) {
        fprintf(stderr, "Lexer error\n");
        free(source);
        return 1;
    }

    // =========================
    // DISPLAY TOKENS
    // =========================

    printf("\n=== TOKENS ===\n");

    t_token* current_token = tokens->head;

    while (current_token) {

        printf(
            "Type: %d | Value: %s | Line: %d\n",
            current_token->type,
            current_token->value,
            current_token->line
        );

        current_token = current_token->next;
    }

    // =========================
    // COMPILATION
    // =========================

    t_bytecode* bytecode = compile(tokens);

    if (!bytecode) {
        fprintf(stderr, "Compiler error\n");
        free(source);
        return 1;
    }

    // =========================
    // DISPLAY BYTECODE
    // =========================

    printf("\n=== BYTECODE ===\n");

    print_bytecode(bytecode);

    // =========================
    // EXECUTION
    // =========================

    printf("\n=== EXECUTION RESULT ===\n");

    t_vm* vm = create_vm();

    execute(vm, bytecode);

    // =========================
    // CLEANUP
    // =========================

    free_vm(vm);
    free(source);

    return 0;
}