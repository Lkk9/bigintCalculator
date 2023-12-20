#include "bigint.h"
#include "notation.h"

#ifndef MATHEXP_H
#define MATHEXP_H

typedef enum { ADD, SUB, MUL, DIV, NONE } operator_t;
typedef enum { NUM = 1, OPERATOR } mexp_item_t;
typedef enum { SPACE, PARENTHESIS_OPEN, PARENTHESIS_CLOSE, NUMBER, MINUS, PLUS, STAR, SLASH, OTHER } token_t;

typedef union {
    operator_t op;
    bi_t *number;
} mexp_data_t;

/**
 * ll_mexp_t - linked list math expression type
 * (big int | operation)
 */
typedef struct ll_mexp_s {
    struct ll_mexp_s *next;
    mexp_data_t data;
    mexp_item_t type;
} ll_mexp_t;

typedef struct mexp_s {
    ll_mexp_t *first;
    ll_mexp_t *last;
} mexp_t;

/**
 * converts string in notation into math expression structure
 */
int mexp_parse(mexp_t *mexp, char *str, notation_t input_type);

/**
 * mexp - linked list with big int numbers and operations in RPN
 * if everything is good evaluates math expression and puts result in result variable
 * othewise returns RC from returncode
 */
int mexp_calculate(mexp_t *mexp, bi_t **result);

/**
 * initilize math expression, should be freed after use
 */
int mexp_init(mexp_t **mexp);

/**
 * free math expression
 */
void mexp_free(mexp_t **mexp);

#endif
