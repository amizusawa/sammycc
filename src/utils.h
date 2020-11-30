#ifndef UTILS_H
#define UTILS_H

#include "defs.h"
#include "data.h"
#include <stdio.h>

void dump_ast(FILE* stream, struct ASTnode* node);
void match(int token, char* what);
void semi();
void ident();
void l_paren();
void r_paren();
void l_brace();
void r_brace();

#endif
