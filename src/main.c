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
        rc_error_inv_args_number(argc - 1);
        return RC_INV_ARG;
    }
    int return_code;
    notation_t notation;
    return_code = notation_get(&notation, argv[1]);
    if (return_code != RC_OK) {
        return rc_error(return_code);
    }

    char input[65536];
    scanf("%65535[^\n]", input);

    mexp_t *mexp;
    if (mexp_init(&mexp) == RC_ERR) {
        return rc_error(RC_ERR);
    }

    return_code = mexp_parse(mexp, input, notation);
    if (return_code != RC_OK) {
        mexp_free(&mexp);
        return return_code;
    }

    bi_t *result;
    if (bi_init(&result) == RC_ERR) {
        mexp_free(&mexp);
        return rc_error(RC_ERR);
    }

    return_code = mexp_calculate(mexp, &result);
    if (return_code != RC_OK) {
        mexp_free(&mexp);
        bi_free(&result);
        return rc_error(return_code);
    }

    return_code = bi_print(result);
    if (return_code != RC_OK) {
        return rc_error(return_code);
    }

    bi_free(&result);
    mexp_free(&mexp);

    return RC_OK;
}
