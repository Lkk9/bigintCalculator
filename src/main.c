#include "bigint.h"
#include "notation.h"
#include "returncode.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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
    // printf("ps: are %p %p %p\n", a.first_digit, a.last_digit, a.info);
    char *cnum = "7";
    bi_init_from_str(&b, "1");
    bi_init_from_str(&a, cnum);
    bi_init(&c);

    ll_bi_show(a);
    // ll_bi_show(b);

    // power
    clock_t begin = clock();
    bi_t *i;
    bi_init_from_str(&i, "0");
    for (; bi_compare(i, a) == -1; bi_add(&i, i, b)) {
        printf("alksd\n");
    }
    clock_t end = clock();

    ll_bi_show(c);

    printf("\n\ntime: %lf\n", (double)(end - begin) / CLOCKS_PER_SEC);
    if (ll_mexp_push(stack, 23) == NULL) {
        return RC_MEM_ERR;
    }
    bi_free(&a);
    bi_free(&b);
    bi_free(&c);

    return RC_OK;
}