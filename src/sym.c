#include "sym.h"

static int next_free_global = 0;

int find_global_sym(char* s) {
    
    for (int i = 0; i < next_free_global; i++) {
        if (*s == *global_sym[i].name && !(strcmp(s, global_sym[i].name))) {
            return i;
        }
    }
    
    return -1;
}

static int new_global_pos() {
    int pos;

    if ((pos = next_free_global++) >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: too many symbols.\n");
        exit(EXIT_FAILURE);
    }

    return pos;
}

int add_global_sym(char* name) {
    
    int y;
    if ((y = find_global_sym(name)) != -1) {
        return y;
    }

    y = new_global_pos();
    global_sym[y].name = strdup(name);
    return y;
}
