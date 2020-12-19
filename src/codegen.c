#include "codegen.h"
#include "gen_x86.h"

static int label() {
    static int id = 1;
    return (id++);
}

int generate_if(struct ASTnode* n) {
    int false_label, end_label;

    false_label = label();
    if (n->right) end_label = label();

    generate_asm(n->left, false_label, n->op);
    freeall_registers();

    generate_asm(n->mid, NOREG, n->op);
    freeall_registers();

    if (n->right) gen_jump(end_label);

    gen_label(false_label);

    if (n->right) {
        generate_asm(n->right, NOREG, n->op);
        freeall_registers();
        gen_label(end_label);
    }

    return NOREG;
}

int generate_while(struct ASTnode* n) {
    int start_label, end_label;

    start_label = label();
    end_label = label();

    gen_label(start_label);
    generate_asm(n->left, end_label, n->op);
    freeall_registers();

    generate_asm(n->right, NOREG, n->op);
    freeall_registers();

    gen_jump(start_label);
    gen_label(end_label);
    return NOREG;
}

int generate_asm(struct ASTnode* n, int reg, int parent_op) {
    int leftreg, rightreg;

    switch(n->op) {
        case A_IF: {
            return generate_if(n);
        }
        case A_WHILE: {
            return generate_while(n);
        }
        case A_GLUE: {
            generate_asm(n->left, NOREG, n->op);
            freeall_registers();
            generate_asm(n->right, NOREG, n->op);
            freeall_registers();
            return NOREG;
        }
    }

    if (n->left) leftreg = generate_asm(n->left, NOREG, n->op);
    if (n->right) rightreg = generate_asm(n->right, leftreg, n->op);

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
        case A_EQ:
        case A_LT:
        case A_GT:
        case A_LE:
        case A_GE:
        case A_NE: {
            if (parent_op == A_IF || parent_op == A_WHILE) {
                return gen_compare_and_jump(n->op, leftreg, rightreg, reg);
            }
            else {
                return gen_compare_and_set(n->op, leftreg, rightreg);
            }
        }
        case A_PRINT: {
            printint(leftreg);
            freeall_registers();
            return NOREG;
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
