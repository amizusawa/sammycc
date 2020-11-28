#include "defs.h"
#define EXTERN_
#include "data.h"
#undef EXTERN_
#include "scan.h"
#include "ast.h"
#include "expr.h"
#include "codegen.h"
#include "stmt.h"
#include "utils.h"
#include <errno.h>

void init_scanner() {
    line = 1;
}

int main(int argc, char** argv) {

    init_scanner();

    in_file = fopen(argv[1], "r");
    if (!in_file) {
        fprintf(stderr, "Failed to open file %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    out_file = fopen("out.s", "w");
    if (!out_file) {
        fprintf(stderr, "Failed to open output file.\n");
        exit(EXIT_FAILURE);
    }

    scan(&current_token);
    generate_preamble();
    statements();
    generate_postamble();

    fclose(in_file);
    fclose(out_file);

    return EXIT_SUCCESS;
}
