#include "../headers/lexer.h"

char peek_char(t_lexer* lexer) {
    if (lexer->source[lexer->pos] == '\0') {
        return '\0'; // end of file
    }
    return lexer->source[lexer->pos];
}

char peek_next_char(t_lexer* lexer) {
    if (lexer->source[lexer->pos + 1] == '\0') {
        return '\0'; // end of file
    }
    return lexer->source[lexer->pos + 1];
}

// advance_pos(): moves forward by one character and returns it
char advance_pos(t_lexer* lexer) {
    if (lexer->source[lexer->pos] == '\0') {
        return '\0'; // end of file
    }
    char c = lexer->source[lexer->pos];
    lexer->pos++;
    if (c == '\n') {
        lexer->line++;
    }
    return c;
}

// check_keyword(): checks whether a lexeme is a keyword and returns its token type
t_token_type check_keyword(const char* lexeme) {
    if (strcmp(lexeme, "let") == 0) return TOKEN_LET;
    if (strcmp(lexeme, "print") == 0) return TOKEN_PRINT;
    if (strcmp(lexeme, "if") == 0) return TOKEN_IF;
    if (strcmp(lexeme, "while") == 0) return TOKEN_WHILE;
    if (strcmp(lexeme, "room") == 0) return TOKEN_ROOM;
    if (strcmp(lexeme, "connect") == 0) return TOKEN_CONNECT;
    if (strcmp(lexeme, "on") == 0) return TOKEN_ON;
    if (strcmp(lexeme, "enter") == 0) return TOKEN_ENTER;
    return TOKEN_ID; // otherwise, it is a regular identifier
}

// scan_identifier(): scans an identifier or a keyword
t_token* scan_identifier(t_lexer* lexer) {
    int start = lexer->pos;
    while (isalnum(peek_char(lexer)) || peek_char(lexer) == '_') {
        advance_pos(lexer);
    }
    int length = lexer->pos - start;
    char* lexeme = (char*)malloc(length + 1);
    strncpy(lexeme, lexer->source + start, length);
    lexeme[length] = '\0';
    t_token_type type = check_keyword(lexeme);
    t_token* token = create_token(type, lexeme, lexer->line);
    free(lexeme);
    return token;
}

// scan_number(): scans an integer literal
t_token* scan_number(t_lexer* lexer) {
    int start = lexer->pos;
    while (isdigit(peek_char(lexer))) {
        advance_pos(lexer);
    }
    int length = lexer->pos - start;
    char* lexeme = (char*)malloc(length + 1);
    strncpy(lexeme, lexer->source + start, length);
    lexeme[length] = '\0';
    t_token* token = create_token(TOKEN_INT, lexeme, lexer->line);
    free(lexeme);
    return token;
}

// scan_string(): scans a string literal
t_token* scan_string(t_lexer* lexer) {
    advance_pos(lexer); // consume opening quote
    int start = lexer->pos;
    while (peek_char(lexer) != '"' && peek_char(lexer) != '\0') {
        if (peek_char(lexer) == '\\' && peek_next_char(lexer) == '"') {
            advance_pos(lexer); // consume backslash
            advance_pos(lexer); // consume escaped quote
        } else {
            advance_pos(lexer);
        }
    }
    if (peek_char(lexer) == '\0') {
        // error: unterminated string
        return create_token(TOKEN_ERROR, "Unterminated string", lexer->line);
    }
    int length = lexer->pos - start;
    char* lexeme = (char*)malloc(length + 1);
    strncpy(lexeme, lexer->source + start, length);
    lexeme[length] = '\0';
    advance_pos(lexer); // consume closing quote
    t_token* token = create_token(TOKEN_STRING, lexeme, lexer->line);
    free(lexeme);
    return token;
}

t_token_list* tokenize(const char* source) {
    t_lexer lexer = {source, (int) strlen(source), 0, 1};
    t_token_list* tokens = create_token_list();

    while (peek_char(&lexer) != '\0') {
        char c = peek_char(&lexer);
        if (isspace(c)) {
            advance_pos(&lexer); // ignore whitespace
        } else if (isalpha(c) || c == '_') {
            append_token(tokens, scan_identifier(&lexer));
        } else if (isdigit(c)) {
            append_token(tokens, scan_number(&lexer));
        } else if (c == '"') {
            append_token(tokens, scan_string(&lexer));
        } else {
            // Handle operators and delimiters
            switch(c) {
                case '+': append_token(tokens, create_token(TOKEN_PLUS, "+", lexer.line)); advance_pos(&lexer); break;
                case '-': append_token(tokens, create_token(TOKEN_MINUS, "-", lexer.line)); advance_pos(&lexer); break;
                case '*': append_token(tokens, create_token(TOKEN_MULT, "*", lexer.line)); advance_pos(&lexer); break;
                case '/': append_token(tokens, create_token(TOKEN_DIV, "/", lexer.line)); advance_pos(&lexer); break;
                case '=':
                    if (peek_next_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(tokens, create_token(TOKEN_EQ, "==", lexer.line));
                    } else {
                        append_token(tokens, create_token(TOKEN_ASSIGN, "=", lexer.line));
                    }
                    advance_pos(&lexer);
                    break;
                case '!':
                    if (peek_next_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(tokens, create_token(TOKEN_NEQ, "!=", lexer.line));
                    } else {
                        advance_pos(&lexer); // consume '!' not followed by '='
                        append_token(tokens, create_token(TOKEN_ERROR, "Unexpected character '!'", lexer.line));
                    }
                    break;
                case '<':
                    if (peek_next_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(tokens, create_token(TOKEN_LTE, "<=", lexer.line));
                    } else {
                        append_token(tokens, create_token(TOKEN_LT, "<", lexer.line));
                    }
                    advance_pos(&lexer);
                    break;
                case '>':
                    if (peek_next_char(&lexer) == '=') {
                        advance_pos(&lexer);
                        append_token(tokens, create_token(TOKEN_GTE, ">=", lexer.line));
                    } else {
                        append_token(tokens, create_token(TOKEN_GT, ">", lexer.line));
                    }
                    advance_pos(&lexer);
                    break;
                case '(': append_token(tokens, create_token(TOKEN_LPAREN, "(", lexer.line)); advance_pos(&lexer); break;
                case ')': append_token(tokens, create_token(TOKEN_RPAREN, ")", lexer.line)); advance_pos(&lexer); break;
                case '{': append_token(tokens, create_token(TOKEN_LBRACE, "{", lexer.line)); advance_pos(&lexer); break;
                case '}': append_token(tokens, create_token(TOKEN_RBRACE, "}", lexer.line)); advance_pos(&lexer); break;
                case ';': append_token(tokens, create_token(TOKEN_SEMICOLON, ";", lexer.line)); advance_pos(&lexer); break;
                case ',': append_token(tokens, create_token(TOKEN_COMMA, ",", lexer.line)); advance_pos(&lexer); break;
                default: {
                    // unrecognized character
                    char error[32];
                    snprintf(error, sizeof(error), "Unexpected character '%c'", c);
                    append_token(tokens, create_token(TOKEN_ERROR, error, lexer.line));
                    advance_pos(&lexer); // consume unrecognized character
                    break;
                }
            }
        }
    }
    append_token(tokens, create_token(TOKEN_EOF, "EOF", lexer.line));
    return tokens;
}