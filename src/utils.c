#include "utils.h"

#define SPACES_PER_INDENT 3

void dump_ast_recursive(FILE* stream, struct ASTnode* node, int num_indents) {
   
    for (int i = 0; i < num_indents * SPACES_PER_INDENT; i++) {
        fprintf(stream, "%c", 32);
    }

    switch (node->op) {
        case A_ADD: {
            fprintf(stream, "ADD\n");
            break;
        }
        case A_SUBTRACT: {
            fprintf(stream, "SUBTRACT\n");
            break;
        }
        case A_MULTIPLY: {
            fprintf(stream, "MULTIPLY\n");
            break;
        }
        case A_DIVIDE: {
            fprintf(stream, "DIVIDE\n");
            break;
        }
        case A_INTLIT: {
            fprintf(stream, "INTLIT:%d\n", node->intvalue);
            break;
        }
        default: {
            fprintf(stderr, "Unknown node operation: %d\n", node->op);
            exit(EXIT_FAILURE);
        }
    }

    if (node->left) {
        dump_ast_recursive(stream, node->left, num_indents+1);
    }
    if (node->right) {
        dump_ast_recursive(stream, node->right, num_indents+1);
    }
}

void dump_ast(FILE* stream, struct ASTnode* node) {
    dump_ast_recursive(stream, node, 0);
}

void match(int token, char* what) {
    
    if (current_token.token == token) {
        scan(&current_token);
    }
    else {
        fprintf(stderr, "%s expected on line %d.\n", what, line);
        exit(EXIT_FAILURE);
    }
}

void semi() {
    match(TOK_SEMICOLON, ";");
}
