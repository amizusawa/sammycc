#include "expr.h"

static int precedence[] = {10, 10, 20, 20, 0,   0};
                        /* +,  -,  *,  /,  EOF, INTLIT*/

static int op_precedence(int tokentype) {
    int prec = precedence[tokentype];
    if (prec == 0) {
        fprintf(stderr, "Syntax error on line %d\n, token %d\n", line, tokentype);
        exit(EXIT_FAILURE);
    }

    return prec;
}

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

struct ASTnode* bin_expr(int prev_prec) {
    struct ASTnode *left, *right;

    left = make_int_lit();

    int tokentype = current_token.token;
    if (tokentype == TOK_EOF) {
        return left;
    }

    while(op_precedence(tokentype) > prev_prec) {
        scan(&current_token);
        right = bin_expr(precedence[tokentype]);
        left = make_node(arith_op(tokentype), left, right, 0);
        tokentype = current_token.token;
        if (tokentype == TOK_EOF) {
            return left;
        }
    }
    return left;
}
