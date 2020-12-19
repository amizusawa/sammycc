#include "gen_x86.h"

#define NUM_REGS 4

static int free_regs[NUM_REGS];
static char* regs[NUM_REGS] = {"%r8", "%r9", "%r10", "%r11"};
static char* b_regs[NUM_REGS] = {"%r8b", "%r9b", "%r10b", "%r11b"};

void freeall_registers() {
    free_regs[0] = free_regs[1] = free_regs[2] = free_regs[3] = 1;
}

static int alloc_register() {
    for (int i = 0; i < NUM_REGS; i++) {
        if (free_regs[i]) {
            free_regs[i] = 0;
            return i;
        }
    }
    fprintf(stderr, "Out of registers.\n");
    exit(EXIT_FAILURE);
}

static void free_register(int reg) {
    if (free_regs[reg] != 0) {
        fprintf(stderr, "Error: freeing non-allocated register. \n");
        exit(EXIT_FAILURE);
    }
    free_regs[reg] = 1;
}

void gen_preamble() {
    freeall_registers();
    fputs(
        "\t.text\n"
        ".LC0:\n"
        "\t.string\t\"%d\\n\"\n"
        "printint:\n"
        "\tpushq\t%rbp\n"
        "\tmovq\t%rsp, %rbp\n"
        "\tsubq\t$16, %rsp\n"
        "\tmovl\t%edi, -4(%rbp)\n"
        "\tmovl\t-4(%rbp), %eax\n"
        "\tmovl\t%eax, %esi\n"
        "\tleaq	.LC0(%rip), %rdi\n"
        "\tmovl	$0, %eax\n"
        "\tcall	printf@PLT\n"
        "\tnop\n"
        "\tleave\n"
        "\tret\n"
        "\n"
        "\t.globl\tmain\n"
        "\t.type\tmain, @function\n"
        "main:\n"
        "\tpushq\t%rbp\n"
        "\tmovq	%rsp, %rbp\n",
    out_file);
}

void gen_postamble()
{
    fputs(
        "\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
    out_file);
}

int gen_load_int(int value) {
    int r = alloc_register();

    fprintf(out_file, "\tmovq\t$%d, %s\n", value, regs[r]);
    return r;
}

int gen_add(int reg1, int reg2) {
    fprintf(out_file, "\taddq\t%s, %s\n", regs[reg1], regs[reg2]);
    free_register(reg1);

    return reg2;
}

int gen_sub(int reg1, int reg2) {
    fprintf(out_file, "\tsubq\t%s, %s\n", regs[reg2], regs[reg1]);
    free_register(reg2);

    return reg1;
}

int gen_mul(int reg1, int reg2) {
    fprintf(out_file, "\timulq\t%s, %s\n", regs[reg1], regs[reg2]);
    free_register(reg1);

    return reg2;
}

int gen_div(int reg1, int reg2) {
    fprintf(out_file, "\tmovq\t%s,%%rax\n", regs[reg1]);
    fprintf(out_file, "\tcqo\n");
    fprintf(out_file, "\tidivq\t%s\n", regs[reg2]);
    fprintf(out_file, "\tmovq\t%%rax,%s\n", regs[reg1]);
    free_register(reg2);
    return reg1;
}

void printint(int reg) {
    fprintf(out_file, "\tmovq\t%s, %%rdi\n", regs[reg]);
    fprintf(out_file, "\tcall\tprintint\n");
    free_register(reg);
}

int gen_load_global_sym(char* identifier) {
    int r = alloc_register();

    fprintf(out_file, "\tmovq\t%s(\%%rip), \%s\n", identifier, regs[r]);
    return r;
}

int gen_store_global_sym(int r, char* identifier) {
    fprintf(out_file, "\tmovq\t%s,%s(\%%rip)\n", regs[r], identifier);
    return r;
}

void gen_global_sym(char* sym) {
    fprintf(out_file, "\t.comm\t%s,8,8\n", sym);
}

static int gen_compare(int reg1, int reg2, char* how) {
    fprintf(out_file, "\tcmpq\t%s, %s\n", regs[reg2], regs[reg1]);
    fprintf(out_file, "\t%s\t%s\n", how, b_regs[reg2]);
    fprintf(out_file, "\tandq\t$255,%s\n", regs[reg2]);
    free_register(reg1);
    return reg2;
}

int gen_eq(int reg1, int reg2) { return gen_compare(reg1, reg2, "sete"); }
int gen_less(int reg1, int reg2) { return gen_compare(reg1, reg2, "setl"); }
int gen_greater(int reg1, int reg2) { return gen_compare(reg1, reg2, "setg"); }
int gen_less_eq(int reg1, int reg2) { return gen_compare(reg1, reg2, "setle"); }
int gen_greater_eq(int reg1, int reg2) { return gen_compare(reg1, reg2, "setge"); }
int gen_not_eq(int reg1, int reg2) { return gen_compare(reg1, reg2, "setne"); }

//                      A_EQ    A_LT    A_GT    A_LE     A_GE     A_NE
char* cmp_list[] =     {"sete", "setl", "setg", "setle", "setge", "setne"};
char* inv_cmp_list[] = {"jne",  "jge",  "jle",  "jg",   "jl",   "je"};

int gen_compare_and_set(int op, int reg1, int reg2) {
    if (op < A_EQ || op > A_NE) {
        fprintf(stderr, "Bad operator on line %d.\n", line);
        exit(EXIT_FAILURE);
    }

    fprintf(out_file, "\tcmpq\t%s, %s\n", regs[reg2], regs[reg1]);
    fprintf(out_file, "\t%s\t%s\n", cmp_list[op - A_EQ], b_regs[reg2]);
    fprintf(out_file, "\tmovzbq\t%s, %s\n", b_regs[reg2], regs[reg2]);

    free_register(reg1);
    return reg2;
}

void gen_label(int label) {
    fprintf(out_file, "L%d:\n", label);
}

void gen_jump(int label) {
    fprintf(out_file, "\tjmp\tL%d\n", label);
}

int gen_compare_and_jump(int op, int reg1, int reg2, int label) {
    if (op < A_EQ || op > A_NE) {
        fprintf(stderr, "Bad operator on line %d.\n", line);
        exit(EXIT_FAILURE);
    }

    fprintf(out_file, "\tcmpq\t%s, %s\n", regs[reg2], regs[reg1]);
    fprintf(out_file, "\t%s\tL%d\n", inv_cmp_list[op - A_EQ], label);
    freeall_registers();

    return NOREG;
}
