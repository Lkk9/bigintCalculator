#include "returncode.h"
#include <stdio.h>

void rc_error_inv_args_name(char *arg_name) { fprintf(stderr, "Arguments: Invalid argument name (%s)\n", arg_name); }
void rc_error_inv_args_number(int cnt) { fprintf(stderr, "Arguments: Requires one argument, instead got (%d)\n", cnt); }
int rc_error(int return_code) {
    switch (return_code) {
    case RC_OK:
    case RC_INV_ARG:
    case RC_NO_SUPPORT_INFIX:
    case RC_NO_SUPPORT_DIVISION:
        break;
    case RC_ERR:
        fprintf(stderr, "Memory allocation error: buy more ram\n");
        break;
    case RC_INV_INPUT:
        fprintf(stderr, "Input: incorrect format for the notation\n");
        break;
    default:
        fprintf(stderr, "Something went wrong\n");
        break;
    }

    return return_code;
}
