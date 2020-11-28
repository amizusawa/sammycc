#ifndef SYM_H
#define SYM_H

#include "data.h"
#include "defs.h"
#include <string.h>
#include <stdio.h>

int find_global_sym(char* s);
int add_global_sym(char* name);

#endif
