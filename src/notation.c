#include "notation.h"
#include "returncode.h"
#include <string.h>

int notation_get(notation_t *notation, char *str) {
    if (strcmp(str, "--revpol") == 0) {
        *notation = REVPOL;
        return RC_OK;
    } else if (strcmp(str, "--infix") == 0) {
        *notation = INFIX;
        return RC_OK;
    }
    rc_error_inv_args_name(str);
    return RC_INV_ARG;
}
