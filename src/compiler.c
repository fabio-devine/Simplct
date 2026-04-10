#include "../headers/compiler.h"

/* ============================================
   UTILITY FUNCTIONS (provided — do not modify)
   ============================================ */

static int check(t_compiler* compiler, t_token_type type) {
    if (!compiler->current) return 0;
    return compiler->current->type == type;
}

static void advance(t_compiler* compiler) {
    compiler->previous = compiler->current;
    if (compiler->current) {
        compiler->current = compiler->current->next;
    }
}

static int match(t_compiler* compiler, t_token_type type) {
    if (check(compiler, type)) {
        advance(compiler);
        return 1;
    }
    return 0;
}

static void expect(t_compiler* compiler, t_token_type type, const char* message) {
    if (!check(compiler, type)) {
        fprintf(stderr, "Compilation Error at line %d: %s\n",
                compiler->current ? compiler->current->line : 0, message);
        compiler->had_error = 1;
        if (compiler->current) {
            advance(compiler);
        }
        return;
    }
    advance(compiler);
}

/* ============================================
   EXPRESSION COMPILATION
   ============================================ */

void compile_primary(t_compiler* compiler) {
    // TODO: handle TOKEN_INT, TOKEN_STRING, TOKEN_ID, TOKEN_LPAREN
    (void)compiler;
}

void compile_factor(t_compiler* compiler) {
    // TODO: compile left operand, then loop on TOKEN_MULT / TOKEN_DIV
    (void)compiler;
}

void compile_term(t_compiler* compiler) {
    // TODO: compile left operand, then loop on TOKEN_PLUS / TOKEN_MINUS
    (void)compiler;
}

void compile_comparison(t_compiler* compiler) {
    // TODO: compile left operand, then loop on TOKEN_LT / TOKEN_GT / TOKEN_LTE / TOKEN_GTE
    (void)compiler;
}

void compile_equality(t_compiler* compiler) {
    // TODO: compile left operand, then loop on TOKEN_EQ / TOKEN_NEQ
    (void)compiler;
}

void compile_expression(t_compiler* compiler) {
    // TODO: entry point for all expressions — call compile_equality
    (void)compiler;
}

/* ============================================
   STATEMENT COMPILATION
   ============================================ */

void compile_let_statement(t_compiler* compiler) {
    // TODO: consume TOKEN_ID, TOKEN_ASSIGN, compile_expression, TOKEN_SEMICOLON, emit OP_STORE_VAR
    (void)compiler;
}

void compile_print_statement(t_compiler* compiler) {
    // TODO: consume TOKEN_LPAREN, compile expression(s), emit OP_PRINT / OP_PRINT_NO_NEWLINE
    (void)compiler;
}

void compile_if_statement(t_compiler* compiler) {
    // TODO: compile condition, emit OP_JUMP_IF_FALSE placeholder, compile block, backpatch
    (void)compiler;
}

void compile_while_statement(t_compiler* compiler) {
    // TODO: save loop_start, compile condition, emit OP_JUMP_IF_FALSE placeholder,
    //       compile block, emit OP_JUMP loop_start, backpatch
    (void)compiler;
}

void compile_block(t_compiler* compiler) {
    // TODO: consume TOKEN_LBRACE, loop compile_statement until TOKEN_RBRACE, consume TOKEN_RBRACE
    (void)compiler;
}

void compile_statement(t_compiler* compiler) {
    // TODO: dispatch to the right compile_*_statement based on the current token
    (void)compiler;
}

/* ============================================
   MAIN COMPILATION FUNCTION
   ============================================ */

t_bytecode* compile(t_token_list* tokens) {
    // TODO:
    // 1. Check tokens is not NULL
    // 2. Initialise t_compiler (current, previous, code, had_error)
    // 3. Loop compile_statement until TOKEN_EOF
    // 4. Emit OP_HALT
    // 5. Return compiler.code (or NULL on error)
    (void)tokens;
    return NULL;
}
