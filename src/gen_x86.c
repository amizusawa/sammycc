#include "gen_x86.h"

#define NUM_REGS 4

static int free_regs[NUM_REGS];
static char* regs[NUM_REGS] = {"%r8", "%r9", "%r10", "%r11"};

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
    if (free_regs[reg] == 1) {
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

int gen_load(int value) {
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
    fprintf(out_file, "\tmovq\t%s. %%rdi\n", regs[reg]);
    fprintf(out_file, "\tcall\tprintint\n");
    free_register(reg);
}
