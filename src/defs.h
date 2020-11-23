
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
    TOK_INTLIT,
    TOK_EOF
};

enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT
};

struct ASTnode {
    int op;
    struct ASTnode* left;
    struct ASTnode* right;
    int intvalue;
};

#endif
