#include "bigint.h"
#include "notation.h"
#include "returncode.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Requires one argument, instead got %d\n", argc - 1);
        return RC_INV_ARG;
    }

    notation_t notation;
    if (strcmp(argv[1], "--revpol") == 0) {
        notation = REVPOL;
    } else if (strcmp(argv[1], "--infix") == 0) {
        notation = INFIX;
    } else {
        fprintf(stderr, "Invalid argument name \"%s\"\n", argv[1]);
        return RC_INV_ARG;
    }
    ll_mexp_t *stack = NULL;
    bi_t *a, *b, *c;
    bi_from_str(a, "-0238");
    bi_from_str(b, "82375789324");
    // bi_add(c, a, b);
    if (ll_mexp_push(stack, 23) == NULL) {
        return RC_MEM_ERR;
    }

    return RC_OK;
}