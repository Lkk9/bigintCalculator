#include <stdint.h>

#ifndef BIGINT_H
#define BIGINT_H

typedef uint32_t digit_base_t; // support 8, 16, 32 bit long int;

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
    uint64_t ones;
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

/* does what it says
 *
 */
int bi_init_from_bi(bi_t **bi_p, bi_t *bi_copy);

/* returns:
 * -1 if a < b
 * 1 if a > b
 * 0 if a == b
 */
int bi_compare(bi_t *a, bi_t *b);

/* assignes sum b+c to variable a
 *
 */
int bi_add(bi_t **a, bi_t *b, bi_t *c);

/* assignes product b*c to variable a
 *
 */
int bi_mul(bi_t **a, bi_t *b, bi_t *c);

/* perfoms left shift or right shift on variable b depending on direction value (0 - left, 1 - right)
 *  stors result in variable a
 */
int bi_shift(bi_t **a, bi_t *b, unsigned direction);

/* prints all nodes (bin data of big int)
 *
 */
void ll_bi_show(bi_t *);

/* adds one digit to the number (in front)
 * destination (0 | 1)
 * destinatino ? at the end : to the start;
 */
int ll_bi_push(bi_t *, digit_base_t, unsigned destination);

/* clears memory after using the number
 *
 */
void bi_free(bi_t **);

/* if bit DNE by index returns -1
 * else return bit (1 | 0)
 * index counts from the right
 *
 */
int bi_get_bit(bi_t *bi, uint64_t index);

#endif