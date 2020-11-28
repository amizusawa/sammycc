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
                    fprintf(stderr, 
                            "Unrecognized symbol %s on line %d.\n", 
                            ident_buffer, line);

                    exit(EXIT_FAILURE);
                }
                
            }
            fprintf(stderr, "Error: unrecognized character %c on line %d.\n", c, line);
            exit(EXIT_FAILURE);
    }

    return 1;
}
