#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <stdio.h>

void generate_code(struct ASTnode* n);
void generate_preamble();
void generate_postamble();
void generate_freeregs();
void generate_printint(int reg);
int generate_asm(struct ASTnode* n, int reg);
void generate_global_sym(char* sym);

#endif
