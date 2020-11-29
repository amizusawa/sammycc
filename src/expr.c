#include "expr.h"

static int precedence[] = {10, // +
                           10, // -
                           20, // *
                           20, // /
                           30, // ==
                           40, // <
                           40, // >
                           40, // <=
                           40, // >=
                           30, // !=
                           0};  // EOF

static int op_precedence(int tokentype) {
    int prec = precedence[tokentype];
    if (prec == 0) {
        fprintf(stderr, "Syntax error on line %d, token %d\n", line, tokentype);
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
        case TOK_EQ: {
            return A_EQ;
        }
        case TOK_LT: {
            return A_LT;
        }
        case TOK_GT: {
            return A_GT;
        }
        case TOK_LE: {
            return A_LE;
        }
        case TOK_GE: {
            return A_GE;
        }
        case TOK_NE: {
            return A_NE;
        }
        default: {
            fprintf(stderr, "Unknown arithmetic operation on line %d.\n", line);
            exit(EXIT_FAILURE);
        }
    }
}

static struct ASTnode* primary() {
    struct ASTnode* n;
    int id;

    switch (current_token.token) {
        case TOK_INTLIT: {
            n = make_leaf(A_INTLIT, current_token.intvalue);
            break;
        }
        case TOK_IDENT: {
            id = find_global_sym(ident_buffer);
            if (id == -1) {
                fprintf(stderr, "Unknown variable %s.\n", ident_buffer);
                exit(EXIT_FAILURE);
            }

            n = make_leaf(A_IDENT, id);
            break;
        }
        default: {
            fprintf(stderr, "Syntax error on line %d\n.", line);
            exit(EXIT_FAILURE);
        }
    }

    scan(&current_token);
    return n;
}

struct ASTnode* bin_expr(int prev_prec) {
    struct ASTnode *left, *right;

    left = primary();

    int tokentype = current_token.token;
    if (tokentype == TOK_SEMICOLON) {
        return left;
    }

    while(op_precedence(tokentype) > prev_prec) {
        scan(&current_token);
        right = bin_expr(precedence[tokentype]);
        left = make_node(arith_op(tokentype), left, right, 0);
        tokentype = current_token.token;
        if (tokentype == TOK_SEMICOLON) {
            return left;
        }
    }
    return left;
}
