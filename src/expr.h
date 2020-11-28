#ifndef EXPR_H
#define EXPR_H

#include "defs.h"
#include "scan.h"
#include "data.h"
#include <stdio.h>
#include "ast.h"

struct ASTnode* bin_expr(int prev_prec);

#endif
