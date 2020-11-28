#include "decl.h"

void var_declaration() {
    
    match(TOK_INT, "int");
    ident();
    add_global_sym(ident_buffer);
    generate_global_sym(ident_buffer);
    semi();
}
