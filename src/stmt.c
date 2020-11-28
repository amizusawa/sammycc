#include "stmt.h"

void statements() {
    struct ASTnode* tree;
    int reg;

    while (1) {
        match(TOK_PRINT, "print");

        tree = bin_expr(0);
        reg = generate_asm(tree);
        generate_printint(reg);
        generate_freeregs();

        semi();
        if (current_token.token == TOK_EOF) {
            return;
        }
    }
}
