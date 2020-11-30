#include "stmt.h"

struct ASTnode* compound_statement();

struct ASTnode* print_statement() {
    struct ASTnode* tree;

    match(TOK_PRINT, "print");

    tree = bin_expr(0);
    tree = make_unary(A_PRINT, tree, 0);

    semi();
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

    semi();

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

struct ASTnode* compound_statement() {
   
    struct ASTnode* left = NULL;
    struct ASTnode* tree;

    l_brace();

    while (1) {
        switch(current_token.token) {
            case TOK_PRINT: {
                tree = print_statement();
                break;
            }
            case TOK_INT: {
                var_declaration();
                tree = NULL;
                break;
            }
            case TOK_IDENT: {
                tree = assignment_statement();
                break;
            }
            case TOK_IF: {
                tree = if_statement();
                break;
            }
            case TOK_RBRACE: {
                r_brace();
                return left;
            }
            default: {
                fprintf(stderr, 
                        "Syntax error %d, on line %d.\n", 
                        current_token.token, line);
                exit(EXIT_FAILURE);
            }
        }
    

        if (tree) {
            if (left == NULL) {
                left = tree;
            }
            else {
                left = make_node(A_GLUE, left, NULL, tree, 0);
            }
        }
    }
}
