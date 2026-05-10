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
        int value = atoi(compiler->previous->lexeme);
        add_push_instruction(compiler->code, value);
    }
    else if (match(compiler, TOKEN_STRING)) {
        t_bytecode_instruction* instr = create_instruction(OP_PUSH_STR, 0, compiler->previous->lexeme);
        add_instruction(compiler->code, instr);
    }
    else if (match(compiler, TOKEN_ID)) {
        add_var_instruction(compiler->code, OP_LOAD_VAR, compiler->previous->lexeme);
    }
    else if (match(compiler, TOKEN_LPAREN)) {
        compile_expression(compiler);
        expect(compiler, TOKEN_RPAREN, "Expected ')' after expression.");
    }
    else {
        fprintf(stderr, "Compilation Error at line %d: Expected expression.\n",
                compiler->current ? compiler->current->line : 0);
        compiler->had_error = 1;
        advance(compiler);
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
    expect(compiler, TOKEN_ID, "Expected variable name after 'let'.");
    char* var_name = compiler->previous->lexeme;
    expect(compiler, TOKEN_ASSIGN, "Expected '=' after variable name.");
    compile_expression(compiler);
    expect(compiler, TOKEN_SEMICOLON, "Expected ';' after let statement.");
    add_var_instruction(compiler->code, OP_STORE_VAR, var_name);
}

void compile_print_statement(t_compiler* compiler) {
    expect(compiler, TOKEN_LPAREN, "Expected '(' after 'print'.");

    compile_expression(compiler);

    while (match(compiler, TOKEN_COMMA)) {
        add_simple_instruction(compiler->code, OP_PRINT_NO_NEWLINE);

        t_bytecode_instruction* space = create_instruction(OP_PUSH_STR, 0, " ");
        add_instruction(compiler->code, space);
        add_simple_instruction(compiler->code, OP_PRINT_NO_NEWLINE);

        compile_expression(compiler);
    }

    add_simple_instruction(compiler->code, OP_PRINT);

    expect(compiler, TOKEN_RPAREN, "Expected ')' after print arguments.");
    expect(compiler, TOKEN_SEMICOLON, "Expected ';' after print statement.");
}

void compile_if_statement(t_compiler* compiler) {
    expect(compiler, TOKEN_LPAREN, "Expected '(' after 'if'.");
    compile_expression(compiler);
    expect(compiler, TOKEN_RPAREN, "Expected ')' after condition.");

    // Émettre un saut conditionnel avec une adresse temporaire (0)
    add_instruction(compiler->code, create_instruction(OP_JUMP_IF_FALSE, 0, NULL));
    int jump_instr_index = compiler->code->count - 1;

    compile_block(compiler);

    // Backpatching : mettre à jour l'adresse du saut vers l'instruction après le bloc
    t_bytecode_instruction* jump_instr = get_instruction_at(compiler->code, jump_instr_index);
    jump_instr->operand = compiler->code->count;
}

void compile_while_statement(t_compiler* compiler) {
    int loop_start = compiler->code->count;
    expect(compiler, TOKEN_LPAREN, "Expected '(' after 'while'.");
    compile_expression(compiler);
    expect(compiler, TOKEN_RPAREN, "Expected ')' after condition.");

    add_instruction(compiler->code, create_instruction(OP_JUMP_IF_FALSE, 0, NULL));
    int exit_jump_index = compiler->code->count - 1;

    compile_block(compiler);

    // Retour au début de la boucle
    add_instruction(compiler->code, create_instruction(OP_JUMP, loop_start, NULL));

    // Backpatching du saut de sortie
    t_bytecode_instruction* exit_jump = get_instruction_at(compiler->code, exit_jump_index);
    exit_jump->operand = compiler->code->count;
}

void compile_block(t_compiler* compiler) {
    expect(compiler, TOKEN_LBRACE, "Expected '{' to start block.");
    while (!check(compiler, TOKEN_RBRACE) && !check(compiler, TOKEN_EOF)) {
        compile_statement(compiler);
    }
    expect(compiler, TOKEN_RBRACE, "Expected '}' after block.");
}

void compile_statement(t_compiler* compiler) {
    if (match(compiler, TOKEN_LET)) compile_let_statement(compiler);
    else if (match(compiler, TOKEN_PRINT)) compile_print_statement(compiler);
    else if (match(compiler, TOKEN_IF)) compile_if_statement(compiler);
    else if (match(compiler, TOKEN_WHILE)) compile_while_statement(compiler);
    else if (check(compiler, TOKEN_LBRACE)) compile_block(compiler);
    else {
        compile_expression(compiler);
        expect(compiler, TOKEN_SEMICOLON, "Expected ';' after expression.");
    }
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