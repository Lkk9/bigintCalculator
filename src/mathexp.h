#include "bigint.h"
#include "notation.h"
// #include <stdbool.h>

#ifndef MATHEXP_H
#define MATHEXP_H

typedef enum { ADD, SUB, MUL, DIV, NONE } operator_t;
typedef enum { NUM = 1, OPERATOR } mexp_item_t;
typedef enum { SPACE, PARENTHESIS_OPEN, PARENTHESIS_CLOSE, NUMBER, MINUS, PLUS, STAR, SLASH, OTHER } token_t;

/*
 * ll_mexp_t - linked list math expression type
 * (big int | operation)
 */
typedef struct ll_mexp_s {
    struct ll_mexp_s *next;
    operator_t op;
    bi_t *number;
    mexp_item_t type;
} ll_mexp_t;

typedef struct mexp_s {
    ll_mexp_t *first;
    ll_mexp_t *last;
} mexp_t;

void mexp_show(mexp_t *stack);
int mexp_push_operator(mexp_t *mexp, operator_t op);
int mexp_enqueue_operator(mexp_t *mexp, operator_t op);
int mexp_push_number(mexp_t *mexp, bi_t *number);
int mexp_enqueue_number(mexp_t *mexp, bi_t *number);

int mexp_parse(mexp_t *mexp, char *str, notation_t input_type);

/* mexp - stack with big int numbers and operations in rpn
 * if everything is good evaluates math expression and puts result in result variable
 * othewise returns RC from returncode
 */
int mexp_calculate(mexp_t *mexp, bi_t **result);

/* returns RC or -1 if stack is empty
 *
 */
int mexp_pop(mexp_t *mexp);

/* call to init math expression
 *
 */
int mexp_init(mexp_t **mexp);

void mexp_free(mexp_t **mexp);
#endif
