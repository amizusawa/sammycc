#include "defs.h"
#define EXTERN_
#include "data.h"
#undef EXTERN_
#include "scan.h"
#include "ast.h"
#include "expr.h"
#include "utils.h"
#include <errno.h>

void init_scanner() {
    line = 1;
}

char *tokstr[] = { "+", "-", "*", "/", "intlit" };

int main(int argc, char** argv) {

    init_scanner();

    in_file = fopen(argv[1], "r");
    if (!in_file) {
        fprintf(stderr, "Failed to open file %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    scan(&current_token);
    struct ASTnode* n = bin_expr(0);
    dump_ast(stdout, n);

    return EXIT_SUCCESS;
}
