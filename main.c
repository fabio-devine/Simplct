#include "headers/io_utils.h"
#include "headers/lexer.h"
#include "headers/compiler.h"
#include "headers/bytecode.h"
#include "headers/vm.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <file.simpl>\n", argv[0]);
        return 1;
    }
    char* source = read_file(argv[1]);

    // =========================
    // 1. AFFICHAGE CODE SOURCE
    // =========================
    printf("=== SOURCE CODE ===\n");
    printf("%s\n", source);

    // =========================
    // 2. LEXER
    // =========================
    printf("\n=== TOKENS ===\n");

    t_lexer* lexer = init_lexer(source);
    t_token_list* tokens = tokenize(lexer);

    print_tokens(tokens);

    // =========================
    // 3. COMPILATION
    // =========================
    printf("\n=== BYTECODE ===\n");

    t_bytecode* bytecode = compile(tokens);

    print_bytecode(bytecode);

    // =========================
    // 4. EXECUTION VM
    // =========================
    printf("\n=== PROGRAM OUTPUT ===\n");

    t_vm* vm = init_vm(bytecode);

    execute(vm);

    return 0;
}
