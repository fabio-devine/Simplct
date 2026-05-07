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
    expect(compiler, TOKEN_ID, "Nom de variable attendu.");
    char* name = strdup(compiler->previous->lexeme);
    expect(compiler, TOKEN_ASSIGN, "'=' attendu.");
    compile_expression(compiler);
    expect(compiler, TOKEN_SEMICOLON, "';' attendu.");
    add_var_instruction(compiler->code, OP_STORE_VAR, name);
    free(name);
}

void compile_print_statement(t_compiler* compiler) {
    expect(compiler, TOKEN_LPAREN, "'(' attendue.");
    compile_expression(compiler);

    while (match(compiler, TOKEN_COMMA)) {
        add_simple_instruction(compiler->code, OP_PRINT_NO_NEWLINE);
        add_var_instruction(compiler->code, OP_PUSH_STR, " ");
        add_simple_instruction(compiler->code, OP_PRINT_NO_NEWLINE);
        compile_expression(compiler);
    }

    expect(compiler, TOKEN_RPAREN, "')' attendue.");
    expect(compiler, TOKEN_SEMICOLON, "';' attendu.");
    add_simple_instruction(compiler->code, OP_PRINT);
}

void compile_if_statement(t_compiler* compiler) {
    expect(compiler, TOKEN_LPAREN, "'(' attendue.");
    compile_expression(compiler);
    expect(compiler, TOKEN_RPAREN, "')' attendue.");

    int jump_address = compiler->code->count;
    add_simple_instruction(compiler->code, OP_JUMP_IF_FALSE);
    compile_block(compiler);
    get_instruction_at(compiler->code, jump_address)->operand = compiler->code->count;
}

void compile_while_statement(t_compiler* compiler) {
    int start_address = compiler->code->count;
    expect(compiler, TOKEN_LPAREN, "'(' attendue.");
    compile_expression(compiler);
    expect(compiler, TOKEN_RPAREN, "')' attendue.");
    int exit_jump_address = compiler->code->count;
    add_simple_instruction(compiler->code, OP_JUMP_IF_FALSE);
    compile_block(compiler);
    t_bytecode_instruction jump_back;
    jump_back.opcode = OP_JUMP;
    jump_back.operand = start_address;
    jump_back.operand_str = NULL;
    add_instruction(compiler->code, &jump_back);

    get_instruction_at(compiler->code, exit_jump_address)->operand = compiler->code->count;
}

void compile_block(t_compiler* compiler) {
    expect(compiler, TOKEN_LBRACE, "'{' attendue.");
    while (!check(compiler, TOKEN_RBRACE) && !check(compiler, TOKEN_EOF)) {
        compile_statement(compiler);
    }
    expect(compiler, TOKEN_RBRACE, "'}' attendue.");
}

void compile_statement(t_compiler* compiler) {
    if (match(compiler, TOKEN_LET)) compile_let_statement(compiler);
    else if (match(compiler, TOKEN_IF)) compile_if_statement(compiler);
    else if (match(compiler, TOKEN_WHILE)) compile_while_statement(compiler);
    else if (match(compiler, TOKEN_PRINT)) compile_print_statement(compiler);
    else if (match(compiler, TOKEN_ID)) {
        // Affectation : x = ...
        char* name = strdup(compiler->previous->lexeme);
        expect(compiler, TOKEN_ASSIGN, "'=' attendu.");
        compile_expression(compiler);
        expect(compiler, TOKEN_SEMICOLON, "';' attendu.");
        add_var_instruction(compiler->code, OP_STORE_VAR, name);
        free(name);
    }
}

/* ============================================
   MAIN COMPILATION FUNCTION
   ============================================ */

t_bytecode* compile(t_token_list* tokens) {
    t_compiler compiler = {0};
    if (tokens) compiler.current = tokens->head;
    compiler.code = create_bytecode();
    while (!check(&compiler, TOKEN_EOF)) {
        compile_statement(&compiler);
        if (compiler.had_error) break;
    }
    add_simple_instruction(compiler.code, OP_HALT);
    return compiler.code;
}