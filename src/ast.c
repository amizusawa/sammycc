#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

struct ASTnode* make_node(int op, struct ASTnode* left, 
                         struct ASTnode* right, int intvalue) {
    struct ASTnode* n;
    
    n = (struct ASTnode*) malloc(sizeof(struct ASTnode));
    if (!n) {
        fprintf(stderr, "Error making new AST node.\n");
        exit(EXIT_FAILURE);
    }

    n->op = op;
    n->left = left;
    n->right = right;
    n->v.intvalue = intvalue;
    return n;
}

struct ASTnode* make_leaf(int op, int intvalue) {
    return make_node(op, NULL, NULL, intvalue);
}

struct ASTnode* make_unary(int op, struct ASTnode* left, int intvalue) {
    return make_node(op, left, NULL, intvalue);
}
