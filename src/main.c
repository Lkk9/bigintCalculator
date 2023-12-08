#include "bigint.h"
#include "notation.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Requires one argument, instead got %d\n", argc - 1);
        return 1;
    }

    notation_t notation;
    if (strcmp(argv[1], "--revpol") == 0) {
        notation = REVPOL;
    } else if (strcmp(argv[1], "--infix") == 0) {
        notation = INFIX;
    } else {
        fprintf(stderr, "Invalid argument name \"%s\"\n", argv[1]);
        return 1;
    }
    ll_mexp_t *stack = NULL;
    bi_t *a, *b;
    bi_from_str(a, "-0234");
    bi_from_str(b, "1208378012789479827349827349729487");
    if (ll_mexp_push(stack, 23) == NULL) {
        return 5;
    }

    return 0;
}