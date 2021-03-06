#include "stmt.h"

struct ASTnode* compound_statement();
static struct ASTnode* single_statement();

struct ASTnode* print_statement() {
    struct ASTnode* tree;

    match(TOK_PRINT, "print");

    tree = bin_expr(0);
    tree = make_unary(A_PRINT, tree, 0);

    return tree;
}

struct ASTnode* assignment_statement() {
    struct ASTnode *left, *right, *tree;
    int id;

    ident();

    if ((id = find_global_sym(ident_buffer)) == -1) {
        fprintf(stderr, 
                "Unidentified variable %s\n on line %d\n.", 
                ident_buffer, line);
        exit(EXIT_FAILURE);
    }

    right = make_leaf(A_LVIDENT, id);
    match(TOK_EQUALS, "=");
    left = bin_expr(0);
    tree = make_node(A_ASSIGN, left, NULL, right, 0);


    return tree;
}

struct ASTnode* if_statement() {
    struct ASTnode *cond_ast, *true_ast, *false_ast = NULL;

    match(TOK_IF, "if");
    l_paren();

    cond_ast = bin_expr(0);

    if (cond_ast->op < A_EQ || cond_ast->op > A_NE) {
        fprintf(stderr, "Bad comparison operator on line %d.\n", line);
        exit(EXIT_FAILURE);
    }

    r_paren();

    true_ast = compound_statement();

    if (current_token.token == TOK_ELSE) {
        scan(&current_token);
        false_ast = compound_statement();
    }

    return make_node(A_IF, cond_ast, true_ast, false_ast, 0);
}

struct ASTnode* while_statement() {
    struct ASTnode *cond_ast, *body_ast;

    match(TOK_WHILE, "while");
    l_paren();

    cond_ast = bin_expr(0);

    if (cond_ast->op < A_EQ || cond_ast->op > A_NE) {
        fprintf(stderr, "Bad comparison operator on line %d. \n", line);
        exit(EXIT_FAILURE);
    }

    r_paren();

    body_ast = compound_statement();

    return make_node(A_WHILE, cond_ast, NULL, body_ast, 0);
}

struct ASTnode* for_statement() {
    struct ASTnode *cond_ast, *body_ast;
    struct ASTnode *pre_ast, *post_ast;
    struct ASTnode *tree;

    match(TOK_FOR, "for");
    l_paren();

    pre_ast = single_statement();
    semi();

    cond_ast = bin_expr(0);

    if (cond_ast->op < A_EQ || cond_ast->op > A_NE) {
        fprintf(stderr, "Bad comparison operator on line %d. \n", line);
        exit(EXIT_FAILURE);
    }

    semi();

    post_ast = single_statement();
    r_paren();

    body_ast = compound_statement();

    tree = make_node(A_GLUE, body_ast, NULL, post_ast, 0);
    tree = make_node(A_WHILE, cond_ast, NULL, tree, 0);
    return make_node(A_GLUE, pre_ast, NULL, tree, 0);
}

static struct ASTnode* single_statement() {
    switch(current_token.token) {
        case TOK_PRINT: {
            return print_statement();
        }
        case TOK_INT: {
            var_declaration();
            return NULL;
        }
        case TOK_IDENT: {
            return assignment_statement();
        }
        case TOK_IF: {
            return if_statement();
        }
        case TOK_WHILE: {
            return while_statement();
        }
        case TOK_FOR: {
            return for_statement();
        }
        default: {
            fprintf(stderr, "Syntax error %d on line %d\n", current_token.token, line);
            exit(EXIT_FAILURE);
        }
    }
}

struct ASTnode* compound_statement() {
   
    struct ASTnode* left = NULL;
    struct ASTnode* tree;

    l_brace();

    while (1) {
        tree = single_statement();

        if (tree != NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN)) {
            semi();
        }

        if (tree != NULL) {
            if (left == NULL) left = tree;
            else left = make_node(A_GLUE, left, NULL, tree, 0);
        }

        if (current_token.token == TOK_RBRACE) {
            r_brace();
            return left;
        }
    }
}
