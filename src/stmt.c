#include "stmt.h"

void print_statement() {
    struct ASTnode* tree;
    int reg;

    match(TOK_PRINT, "print");

    tree = bin_expr(0);
    reg = generate_asm(tree, -1);
    generate_printint(reg);
    generate_freeregs();

    semi();
}

void assignment_statement() {
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
    tree = make_node(A_ASSIGN, left, right, 0);
    generate_asm(tree, -1);
    generate_freeregs();

    semi();
}

void statements() {
    
    while (1) {
        switch(current_token.token) {
            case TOK_PRINT: {
                print_statement();
                break;
            }
            case TOK_INT: {
                var_declaration();
                break;
            }
            case TOK_IDENT: {
                assignment_statement();
                break;
            }
            case TOK_EOF: {
                return;
            }
            default: {
                fprintf(stderr, 
                        "Syntax error %d, on line %d.\n", 
                        current_token.token, line);
                exit(EXIT_FAILURE);
            }
        }
    }
}
