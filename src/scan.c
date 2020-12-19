#include "scan.h"
#include "defs.h"
#include "data.h"

static int next() {
    int c;

    c = fgetc(in_file);
    if (c == '\n') {
        line++;
    }
    return c;
}

static int skip() {
    int c; 

    c = next();
    while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
        c = next();
    }

    return c;
}

static int scanident(int c, char* buf, int lim) {
    int i = 0;

    while (isalpha(c) || isdigit(c) || c == '_') {
        if (i == lim - 1) {
            fprintf(stderr, "Error: identifier on line %d too long.", line);
            exit(EXIT_FAILURE);
        }
        buf[i++] = c;
        c = next();
    }

    ungetc(c, in_file);
    buf[i] = '\0';
    return i;
}

static int char_to_int(int c) {

    if (c >= '0' && c <= '9') {
        return (c - '0');
    }
    return -1;

}

static int scanint(int c) {
    int val, k;
    val = 0;

    while ((k = char_to_int(c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }

    ungetc(c, in_file);
    return val;
}

static int keyword(char* s) {
    switch (*s) {
        case 'p': {
            if (!strcmp(s, "print")) {
                return TOK_PRINT;
            }
            break;
        }
        case 'i': {
            if (!strcmp(s, "int")) {
                return TOK_INT;
            }
            if (!strcmp(s, "if")) {
                return TOK_IF;
            }
            break;
        }
        case 'e': {
            if (!strcmp(s, "else")) {
                return TOK_ELSE;
            }
            break;
        }
        case 'w': {
            if (!strcmp(s, "while")) {
                return TOK_WHILE;
            }
            break;
        }
    }
    return 0;
}

int scan(struct token* t) {
    int c, tokentype;

    c = skip();

    switch (c) {
        case EOF: {
            t->token = TOK_EOF;
            return 0;
        }
        case '+': {
            t->token = TOK_PLUS;
            break;
        }
        case '-': {
            t->token = TOK_MINUS;
            break;
        }
        case '*': {
            t->token = TOK_STAR;
            break;
        }
        case '/': {
            t->token = TOK_SLASH;
            break;
        }
        case ';': {
            t->token = TOK_SEMICOLON;
            break;
        }
        case '{': {
            t->token = TOK_LBRACE;
            break;
        }
        case '}': {
            t->token = TOK_RBRACE;
            break;
        }
        case '(': {
            t->token = TOK_LPAREN;
            break;
        }
        case ')': {
            t->token = TOK_RPAREN;
            break;
        }
        case '=': {
            if ((c = next()) == '=') {
                t->token = TOK_EQ;
            }
            else {
                ungetc(c, in_file);
                t->token = TOK_EQUALS;
            }
            break;
        }
        case '<': {
            if ((c = next()) == '=') {
                t->token = TOK_LE;
            }
            else {
                ungetc(c, in_file);
                t->token = TOK_LT;
            }
            break;
        }
        case '>': {
            if ((c = next()) == '=') {
                t->token = TOK_GE;
            }
            else {
                ungetc(c, in_file);
                t->token = TOK_GT;
            }
            break;
        }
        case '!': {
            if ((c = next()) == '=') {
                t->token = TOK_NE;
            }
            else {
                fprintf(stderr, "Unrecognized character %c on line %d.\n", c, line);
                exit(EXIT_FAILURE);
            }
            break;
        }
        default:
            if (isdigit(c)) {
                t->intvalue = scanint(c);
                t->token = TOK_INTLIT;
                break;
            }
            else if (isalpha(c) || c == '_') {
                scanident(c, ident_buffer, MAX_TEXTLEN);
                
                if ((tokentype = keyword(ident_buffer))) {
                    t->token = tokentype;
                    break;
                }
                else {
                    t->token = TOK_IDENT;
                    break;
                }
                
            }
            fprintf(stderr, "Error: unrecognized character %c on line %d.\n", c, line);
            exit(EXIT_FAILURE);
    }

    return 1;
}
