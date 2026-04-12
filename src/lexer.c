#include "../headers/lexer.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// peek_char(): returns the current character without advancing the position
char peek_char(t_lexer* lexer) {
    if (!lexer || !lexer->source)
        return '\0';
    if (lexer->pos >= lexer->length)
        return '\0';
    return lexer->source[lexer->pos];
}

// peek_next_char(): returns the next character without advancing the position
char peek_next_char(t_lexer* lexer) {
    if (!lexer || !lexer->source)
        return '\0';
    if (lexer->pos + 1 >= lexer->length)
        return '\0';
    return lexer->source[lexer->pos + 1];
}

// advance_pos(): moves forward by one character and returns it
char advance_pos(t_lexer* lexer) {
    if (!lexer || !lexer->source)
        return '\0';
    char c = lexer->source[lexer->pos];
    if (c == '\0')
        return '\0';
    lexer->pos++;
    if (c == '\n')
        lexer->line++;
    return c;
}

// check_keyword(): checks whether a lexeme is a keyword and returns its token type
t_token_type check_keyword(const char* lexeme) {
    if (strcmp(lexeme, "let")   == 0) return TOKEN_LET;
    if (strcmp(lexeme, "print") == 0) return TOKEN_PRINT;
    if (strcmp(lexeme, "if")    == 0) return TOKEN_IF;
    if (strcmp(lexeme, "while") == 0) return TOKEN_WHILE;
    return TOKEN_ID;
}

// scan_identifier(): scans an identifier or a keyword
t_token* scan_identifier(t_lexer* lexer) {
    int start = lexer->pos;
    int line  = lexer->line;
    while (isalpha(peek_char(lexer)))
        advance_pos(lexer);
    int len = lexer->pos - start;
    char* lexeme = (char*)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    lexeme[len] = '\0';
    t_token_type type = check_keyword(lexeme);
    t_token* token = create_token(type, lexeme, line);
    free(lexeme);
    return token;
}

// scan_number(): scans an integer literal
t_token* scan_number(t_lexer* lexer) {
    int start = lexer->pos;
    int line  = lexer->line;
    while (isdigit(peek_char(lexer)))
        advance_pos(lexer);
    int len = lexer->pos - start;
    char* lexeme = (char*)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    lexeme[len] = '\0';
    t_token* token = create_token(TOKEN_INT, lexeme, line);
    token->value = atoi(lexeme);
    free(lexeme);
    return token;
}

// scan_string(): scans a string literal
t_token* scan_string(t_lexer* lexer) {
    int line = lexer->line;
    advance_pos(lexer);
    int start = lexer->pos;
    while (peek_char(lexer) != '"' && peek_char(lexer) != '\0')
        advance_pos(lexer);
    if (peek_char(lexer) == '\0')
        return create_token(TOKEN_ERROR, "Unterminated string literal", line);
    int len = lexer->pos - start;
    char* lexeme = (char*)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    lexeme[len] = '\0';
    advance_pos(lexer);
    t_token* token = create_token(TOKEN_STRING, lexeme, line);
    free(lexeme);
    return token;
}

t_token_list* tokenize(const char* source) {
    t_lexer lexer;
    lexer.source = source;
    lexer.length = (int)strlen(source);
    lexer.pos    = 0;
    lexer.line   = 1;
    t_token_list* list = create_token_list();
    while (peek_char(&lexer) != '\0')
    {
        char c = peek_char(&lexer);
        int line = lexer.line;
        if (isspace(c))
            advance_pos(&lexer);
        else if (isalpha(c))
            append_token(list, scan_identifier(&lexer));
        else if (isdigit(c))
            append_token(list, scan_number(&lexer));
        else if (c == '"')
            append_token(list, scan_string(&lexer));
        else
        {
            advance_pos(&lexer);
            switch (c)
            {
                case '+': append_token(list, create_token(TOKEN_PLUS,      "+", line)); break;
                case '-': append_token(list, create_token(TOKEN_MINUS,     "-", line)); break;
                case '*': append_token(list, create_token(TOKEN_MULT,      "*", line)); break;
                case '/': append_token(list, create_token(TOKEN_DIV,       "/", line)); break;
                case '=':
                    if (peek_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(list, create_token(TOKEN_EQ, "==", line));
                    } else {
                        append_token(list, create_token(TOKEN_ASSIGN, "=", line));
                    }
                    break;
                case '!':
                    if (peek_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(list, create_token(TOKEN_NEQ, "!=", line));
                    } else {
                        append_token(list, create_token(TOKEN_ERROR, "!", line));
                    }
                    break;
                case '<':
                    if (peek_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(list, create_token(TOKEN_LTE, "<=", line));
                    } else {
                        append_token(list, create_token(TOKEN_LT, "<", line));
                    }
                    break;
                case '>':
                    if (peek_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(list, create_token(TOKEN_GTE, ">=", line));
                    } else {
                        append_token(list, create_token(TOKEN_GT, ">", line));
                    }
                    break;
                case '(': append_token(list, create_token(TOKEN_LPAREN,    "(", line)); break;
                case ')': append_token(list, create_token(TOKEN_RPAREN,    ")", line)); break;
                case '{': append_token(list, create_token(TOKEN_LBRACE,    "{", line)); break;
                case '}': append_token(list, create_token(TOKEN_RBRACE,    "}", line)); break;
                case ';': append_token(list, create_token(TOKEN_SEMICOLON, ";", line)); break;
                case ',': append_token(list, create_token(TOKEN_COMMA,     ",", line)); break;
                default:
                {
                    char unknown[2] = { c, '\0' };
                    append_token(list, create_token(TOKEN_ERROR, unknown, line));
                    break;
                }
            }
        }
    }
    append_token(list, create_token(TOKEN_EOF, "EOF", lexer.line));
    return list;
}