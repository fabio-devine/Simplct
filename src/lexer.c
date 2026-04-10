#include "../headers/lexer.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// peek_char(): returns the current character without advancing the position
char peek_char(t_lexer* lexer) {
    return lexer->input[lexer->pos];
}

// peek_next_char(): returns the next character without advancing the position
char peek_next_char(t_lexer* lexer) {
    return lexer->input[lexer->pos+1];
}

// advance_pos(): moves forward by one character and returns it
char advance_pos(t_lexer* lexer) {
    return lexer->input[lexer->pos++];
}

// check_keyword(): checks whether a lexeme is a keyword and returns its token type
t_token_type check_keyword(const char* lexeme) {
    if (strcmp(lexeme, "let") == 0)
        return TOKEN_LET;
    if (strcmp(lexeme, "print") == 0)
        return TOKEN_PRINT;
    return TOKEN_EOF;
}

// scan_identifier(): scans an identifier or a keyword
t_token* scan_identifier(t_lexer* lexer) {
    t_token* scan_identifier(t_lexer* lexer) {
        int start = lexer->pos;
        while (isalpha(peek_char(lexer)) || isdigit(peek_char(lexer))) {
            advance_pos(lexer);
        }
        int length = lexer->pos - start;
        char* lexeme = malloc(sizeof(char) * (length + 1));
        if (!lexeme)
            return NULL;
        strncpy(lexeme, lexer->input + start, length);
        lexeme[length] = '\0';
        t_token_type type = check_keyword(lexeme);
        if (type == TOKEN_EOF)
            type = TOKEN_IDENTIFIER;
        t_token* token = create_token(type, lexeme);
        if (!token)
            return NULL;
        return token;
    }
}

// scan_number(): scans an integer literal
t_token* scan_number(t_lexer* lexer) {
    t_token* scan_number(t_lexer* lexer) {
        int start = lexer->pos;
        while (isdigit(peek_char(lexer))) {
            advance_pos(lexer);
        }
        int length = lexer->pos - start;
        char* lexeme = malloc(sizeof(char) * (length + 1));
        if (!lexeme)
            return NULL;
        strncpy(lexeme, lexer->input + start, length);
        lexeme[length] = '\0';
        t_token* token = create_token(TOKEN_NUMBER, lexeme);
        if (!token)
            return NULL;
        return token;
    }
}

// scan_string(): scans a string literal
t_token* scan_string(t_lexer* lexer) {
    int start = lexer->pos + 1;
    advance_pos(lexer);
    while (peek_char(lexer) != '"' && peek_char(lexer) != '\0') {
        advance_pos(lexer);
    }
    if (peek_char(lexer) == '\0')
        return NULL;
    int length = lexer->pos - start;
    char* lexeme = malloc(sizeof(char) * (length + 1));
    if (!lexeme)
        return NULL;
    strncpy(lexeme, lexer->input + start, length);
    lexeme[length] = '\0';
    advance_pos(lexer);
    t_token* token = create_token(TOKEN_STRING, lexeme);
    if (!token)
        return NULL;
    return token;
}

t_token_list* tokenize(const char* source) {
    return NULL;
}