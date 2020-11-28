
#ifndef DATA_H
#define DATA_H

#ifndef EXTERN_
#define EXTERN_ extern
#endif

#include <stdio.h>
#include "scan.h"
#include "defs.h"

#define MAX_TEXTLEN 512
#define MAX_SYMBOLS 1024

EXTERN_ int line;
EXTERN_ FILE* in_file;
EXTERN_ FILE* out_file;
EXTERN_ struct token current_token;
EXTERN_ char ident_buffer[MAX_TEXTLEN + 1];
EXTERN_ struct symtable global_sym[MAX_SYMBOLS];

#endif
