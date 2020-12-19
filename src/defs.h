
#ifndef DEFS_H
#define DEFS_H

struct token {
    int token;
    int intvalue;
};

enum {
    // Operators
    TOK_PLUS,
    TOK_MINUS,
    TOK_STAR,
    TOK_SLASH,
    TOK_EQ,
    TOK_LT,
    TOK_GT,
    TOK_LE,
    TOK_GE,
    TOK_NE,

    // Special characters
    TOK_EOF,
    TOK_SEMICOLON,
    TOK_EQUALS,
    TOK_LBRACE,
    TOK_RBRACE,
    TOK_LPAREN,
    TOK_RPAREN,

    // Keywords
    TOK_PRINT,
    TOK_INT,
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_FOR,

    // Identifiers
    TOK_IDENT,
    TOK_INTLIT
};

enum {
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_EQ,
    A_LT,
    A_GT,
    A_LE,
    A_GE,
    A_NE,
    A_INTLIT,
    A_IDENT,
    A_LVIDENT,
    A_ASSIGN,
    A_GLUE,
    A_IF,
    A_WHILE,
    A_FOR,
    A_PRINT
};

struct ASTnode {
    int op;
    struct ASTnode* left;
    struct ASTnode* mid;
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
