#ifndef GEN_X86_H
#define GEN_X86_H

#include <stdio.h>
#include <stdlib.h>
#include "data.h"

void freeall_registers();
void gen_preamble();
void gen_postamble();
int gen_load(int value);
int gen_add(int reg1, int reg2);
int gen_sub(int reg1, int reg2);
int gen_mul(int reg1, int reg2);
int gen_div(int reg1, int reg2);
void printint(int reg);

#endif