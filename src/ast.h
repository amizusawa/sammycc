#ifndef AST_H
#define AST_H

#include "defs.h"

struct ASTnode* make_node(int op, struct ASTnode* left, struct ASTnode* right, int intvalue);
struct ASTnode* make_leaf(int op, int intvalue);
struct ASTnode* make_unary(int op, struct ASTnode* left, int intvalue);
#endif
