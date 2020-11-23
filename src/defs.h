
#ifndef DEFS_H
#define DEFS_H

struct token {
    int token;
    int intvalue;
};

enum {
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_INTLIT
};

#endif
