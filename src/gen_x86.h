#ifndef GEN_X86_H
#define GEN_X86_H

#include <stdio.h>
#include <stdlib.h>
#include "data.h"

#define NOREG -1

void freeall_registers();
void gen_preamble();
void gen_postamble();
int gen_load_int(int value);
int gen_add(int reg1, int reg2);
int gen_sub(int reg1, int reg2);
int gen_mul(int reg1, int reg2);
int gen_div(int reg1, int reg2);
int gen_eq(int reg1, int reg2);
int gen_less(int reg1, int reg2);
int gen_greater(int reg1, int reg2);
int gen_less_eq(int reg1, int reg2);
int gen_greater_eq(int reg1, int reg2);
int gen_not_eq(int reg1, int reg2);
void printint(int reg);
int gen_load_global_sym(char* identifier);
int gen_store_global_sym(int r, char* identifier);
void gen_global_sym(char* sym);
int gen_compare_and_set(int op, int reg1, int reg2);
int gen_compare_and_jump(int op, int reg1, int reg2, int label);
void gen_jump(int label);
void gen_label(int label);

#endif
