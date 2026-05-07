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
    if (match(compiler, TOKEN_INT)) {
        add_push_instruction(compiler->code, atoi(compiler->previous->lexeme));
    }
    else if (match(compiler, TOKEN_STRING)) {
        // Remplacement par add_var_instruction pour utiliser OP_PUSH_STR
        add_var_instruction(compiler->code, OP_PUSH_STR, compiler->previous->lexeme);
    }
    else if (match(compiler, TOKEN_ID)) {
        add_var_instruction(compiler->code, OP_LOAD_VAR, compiler->previous->lexeme);
    }
    else if (match(compiler, TOKEN_LPAREN)) {
        compile_expression(compiler);
        expect(compiler, TOKEN_RPAREN, "Parenthèse fermante ')' attendue.");
    }
    else {
        compiler->had_error = 1;
    }
}

void compile_factor(t_compiler* compiler) {
    compile_primary(compiler);
    while (match(compiler, TOKEN_MULT) || match(compiler, TOKEN_DIV)) {
        t_token_type op = compiler->previous->type;
        compile_primary(compiler);
        add_simple_instruction(compiler->code, (op == TOKEN_MULT) ? OP_MUL : OP_DIV);
    }
}

void compile_term(t_compiler* compiler) {
    compile_factor(compiler);
    while (match(compiler, TOKEN_PLUS) || match(compiler, TOKEN_MINUS)) {
        t_token_type op = compiler->previous->type;
        compile_factor(compiler);
        add_simple_instruction(compiler->code, (op == TOKEN_PLUS) ? OP_ADD : OP_SUB);
    }
}

void compile_comparison(t_compiler* compiler) {
    compile_term(compiler);
    while (match(compiler, TOKEN_LT) || match(compiler, TOKEN_GT) ||
           match(compiler, TOKEN_LTE) || match(compiler, TOKEN_GTE)) {
        t_token_type op = compiler->previous->type;
        compile_term(compiler);
        if (op == TOKEN_LT) add_simple_instruction(compiler->code, OP_LT);
        else if (op == TOKEN_GT) add_simple_instruction(compiler->code, OP_GT);
        else if (op == TOKEN_LTE) add_simple_instruction(compiler->code, OP_LTE);
        else add_simple_instruction(compiler->code, OP_GTE);
           }
}

void compile_equality(t_compiler* compiler) {
    compile_comparison(compiler);
    while (match(compiler, TOKEN_EQ) || match(compiler, TOKEN_NEQ)) {
        t_token_type op = compiler->previous->type;
        compile_comparison(compiler);
        add_simple_instruction(compiler->code, (op == TOKEN_EQ) ? OP_EQ : OP_NEQ);
    }
}

void compile_expression(t_compiler* compiler) {
    compile_equality(compiler);
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