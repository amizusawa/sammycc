#ifndef UTILS_H
#define UTILS_H

#include "defs.h"
#include "data.h"
#include <stdio.h>

void dump_ast(FILE* stream, struct ASTnode* node);
void match(int token, char* what);
void semi();
void ident();

#endif
