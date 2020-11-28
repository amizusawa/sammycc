
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
    TOK_EOF,
    TOK_SEMICOLON,
    TOK_PRINT,
    TOK_EQUALS,
    TOK_INT,
    TOK_IDENT
};

enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT,
    A_IDENT,
    A_LVIDENT,
    A_ASSIGN
};

struct ASTnode {
    int op;
    struct ASTnode* left;
    struct ASTnode* right;
    union {
        int intvalue;
        int id;
    } v;
};

struct symtable {
    char* name;
};

#endif
