#include "scan.h"
#include "defs.h"
#include "data.h"

static int next() {
    int c;

    if (putback) {
        c = putback;
        putback = 0;
        return c;
    }

    c = fgetc(in_file);
    if (c == '\n') {
        line++;
    }
    return c;
}

static int skip() {
    int c; 

    c = next();
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
        c = next();
    }

    return c;
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

int scan(struct token* t) {
    int c;

    c = skip();

    switch (c) {
        case EOF: {
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
        default:
            if (isdigit(c)) {
                t->intvalue = scanint(c);
                t->token = TOK_INTLIT;
                break;
            }
            printf("Error: unrecognized character %c on line %d.\n", c, line);
            exit(EXIT_FAILURE);
    }

    return 1;
}
