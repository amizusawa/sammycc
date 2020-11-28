#include "codegen.h"
#include "gen_x86.h"

int generate_asm(struct ASTnode* n, int reg) {
    int leftreg, rightreg;
    if (n->left) leftreg = generate_asm(n->left, -1);
    if (n->right) rightreg = generate_asm(n->right, leftreg);

    switch(n->op) {
        case A_ADD: {
            return gen_add(leftreg, rightreg);
        }
        case A_SUBTRACT: {
            return gen_sub(leftreg, rightreg);
        }
        case A_MULTIPLY: {
            return gen_mul(leftreg, rightreg);
        }
        case A_DIVIDE: {
            return gen_div(leftreg, rightreg);
        }
        case A_INTLIT: {
            return gen_load_int(n->v.intvalue);
        }
        case A_IDENT: {
            return gen_load_global_sym(global_sym[n->v.id].name);
        }
        case A_LVIDENT: {
            return gen_store_global_sym(reg, global_sym[n->v.id].name);
        }
        case A_ASSIGN: {
            return rightreg;
        }
        default: {
            fprintf(stderr, "Unknown AST operator %d\n", n->op);
            exit(EXIT_FAILURE);
        }
    }
}

/*
void generate_code(struct ASTnode* n) {
    int reg;
    gen_preamble();
    reg = gen_asm(n);
    printint(reg);
    gen_postamble();
}
*/

void generate_preamble() {
    gen_preamble();
}

void generate_postamble() {
    gen_postamble();
}

void generate_freeregs() {
    freeall_registers();
}

void generate_printint(int reg) {
    printint(reg);
}

void generate_global_sym(char* sym) {
    gen_global_sym(sym);
}
