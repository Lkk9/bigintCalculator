#include "bigint.h"
#include "mathexp.h"
#include "notation.h"
#include "returncode.h"
#include <stdio.h>
#include <stdlib.h>
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
    char input[4096];
    scanf("%4095[^\n]", input);

    mexp_t *mexp;
    if (mexp_init(&mexp) == RC_ERR) {
        fprintf(stderr, "Memory error: buy more memory\n");
        return RC_ERR;
    }

    int return_code = mexp_parse(mexp, input, notation);
    if (return_code != RC_OK) {
        return return_code;
    }

    bi_t *result;
    if (bi_init(&result) == RC_ERR) {
        return RC_ERR;
    }

    int ret = mexp_calculate(mexp, &result);
    if (ret != RC_OK) {
        return ret;
    }
    bi_show(result);
    bi_print(result, 10);

    bi_free(&result);

    return RC_OK;
}
