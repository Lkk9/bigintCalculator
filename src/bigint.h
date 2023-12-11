#include <stdint.h>

#ifndef BIGINT_H
#define BIGINT_H

typedef uint8_t digit_base_t;

#define DIGIT_BASE_SIZE (sizeof(digit_base_t) * 8)

typedef struct ll_bi_s {
    struct ll_bi_s *next_digit;
    struct ll_bi_s *prev_digit;
    digit_base_t digit;
} ll_bi_t;

typedef struct bi_s {
    ll_bi_t *first_digit;
    ll_bi_t *last_digit;
    uint64_t total_digits;
    uint8_t is_negative;
} bi_t;

/*
 * initializes big int
 */
int bi_init(bi_t **);

/*
 * return values:
 * 0 if everything is ok
 * 4 if not [-0123456789]
 * 6 if only [-]
 * 5 if memory allocation failed
 */
int bi_init_from_str(bi_t **, char *);

/* returns:
 * -1 if a < b
 * 1 if a > b
 * 0 if a == b
 */
int bi_compare(bi_t *a, bi_t *b);

/*
 *
 */
int bi_add(bi_t *, bi_t *, bi_t *);

/* prints all nodes (bin data of big int)
 *
 */
void ll_bi_show(bi_t *);

/* adds one digit (base digit_base) to the number
 *
 */
int ll_bi_push(bi_t *, digit_base_t);

/* clears memory after using the number
 *
 */
void bi_free(bi_t **);

#endif