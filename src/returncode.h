#ifndef RETURNCODE_H
#define RETURNCODE_H

#define RC_OK (0)
#define RC_INV_ARG (1)
#define RC_NO_SUPPORT_INFIX (2)
#define RC_NO_SUPPORT_DIVISION (3)
#define RC_INV_INPUT (4)
#define RC_ERR (5)

void rc_error_inv_args_name(char *arg_name);
void rc_error_inv_args_number(int cnt);
int rc_error(int return_code);

#endif
