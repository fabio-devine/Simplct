#include "../headers/lexer.h"

// peek_char(): returns the current character without advancing the position
char peek_char(t_lexer* lexer) {
    return '0';
}

// peek_next_char(): returns the next character without advancing the position
char peek_next_char(t_lexer* lexer) {
    return '0';
}

// advance_pos(): moves forward by one character and returns it
char advance_pos(t_lexer* lexer) {
    return '0';
}

// check_keyword(): checks whether a lexeme is a keyword and returns its token type
t_token_type check_keyword(const char* lexeme) {
    return TOKEN_EOF;
}

// scan_identifier(): scans an identifier or a keyword
t_token* scan_identifier(t_lexer* lexer) {
    return NULL;
}

// scan_number(): scans an integer literal
t_token* scan_number(t_lexer* lexer) {
    return NULL;
}

// scan_string(): scans a string literal
t_token* scan_string(t_lexer* lexer) {
    return NULL;
}

t_token_list* tokenize(const char* source) {
    return NULL;
}