#include "expr.h"

int arith_op(int token) {
    switch (token) {
        case TOK_PLUS: {
            return A_ADD;
        }
        case TOK_MINUS: {
            return A_SUBTRACT;
        }
        case TOK_STAR: {
            return A_MULTIPLY;
        }
        case TOK_SLASH: {
            return A_DIVIDE;
        }
        default: {
            fprintf(stderr, "Unknown arithmetic operation on line %d.\n", line);
            exit(EXIT_FAILURE);
        }
    }
}

static struct ASTnode* make_int_lit() {
    struct ASTnode* n;

    switch (current_token.token) {
        case TOK_INTLIT: {
            n = make_leaf(A_INTLIT, current_token.intvalue);
            scan(&current_token);
            return n;
        }
        default: {
            fprintf(stderr, "Syntax error on line %d\n.", line);
            exit(EXIT_FAILURE);
        }
    }
}

struct ASTnode* bin_expr() {
    struct ASTnode *n, *left, *right;

    left = make_int_lit();

    if (current_token.token == TOK_EOF) {
        return left;
    }

    int nodetype = arith_op(current_token.token);
    scan(&current_token);
    right = bin_expr();
    n = make_node(nodetype, left, right, 0);

    return n;
}
