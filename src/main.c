#include "defs.h"
#define EXTERN_
#include "data.h"
#undef EXTERN_
#include "scan.h"
#include <errno.h>

void init_scanner() {
    line = 1;
    putback = 0;
}

char *tokstr[] = { "+", "-", "*", "/", "intlit" };

void scan_file() {
    struct token t;

    while (scan(&t)) {
        if (t.token == TOK_INTLIT) {
            printf("Token int literal: %d\n", t.intvalue);
        }
        else {
            printf("Token read. %s\n", tokstr[t.token]);
        }
    }
}

int main(int argc, char** argv) {

    init_scanner();

    in_file = fopen(argv[1], "r");
    if (!in_file) {
        fprintf(stderr, "Failed to open file %s.\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    scan_file();

    return EXIT_SUCCESS;
}
