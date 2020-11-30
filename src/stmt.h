#ifndef STMT_H
#define STMT_H

#include "data.h"
#include "ast.h"
#include "defs.h"
#include "scan.h"
#include "expr.h"
#include "utils.h"
#include "sym.h"
#include "codegen.h"
#include "decl.h"
#include <stdio.h>

struct ASTnode* compound_statement();

#endif
