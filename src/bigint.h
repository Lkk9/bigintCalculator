#include <stdint.h>

#ifndef BIGINT_H
#define BIGINT_H

typedef uint32_t digit_base_t; // supports unsigned 8, 16, 32 bit int;

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
    int8_t is_negative;
    uint64_t ones;
} bi_t;

/**
 * prints big int in base 10
 */
int bi_print(bi_t *bi);

/**
 * bi1 < bi2 return -1
 * bi1 == bi2 return 0
 * bi1 > bi2 return 1
 *
 * do_abs (1 | 0)
 * compare absolute values if do_abs == 1
 */
int bi_compare(bi_t *bi1, bi_t *bi2, unsigned do_abs);

/**
 * result = bi1 + bi2
 * mutates result
 */
int bi_add(bi_t **result, bi_t *bi1, bi_t *bi2);

/**
 * result = bi1 - bi2
 * mutates result
 */
int bi_sub(bi_t **result, bi_t *bi1, bi_t *bi2);

/**
 * result = bi1 * bi2
 * mutates result
 */
int bi_mul(bi_t **result, bi_t *bi1, bi_t *bi2);

/**
 * result = bi_copy
 * mutates result
 */
int bi_set_bi(bi_t **result, bi_t *bi_copy);

/**
 * initializes big int, should be freed after use
 */
int bi_init(bi_t **bi_p);

/**
 * frees big int
 */
void bi_free(bi_t **bi_p);

/**
 * initializes big int from existing big int
 */
int bi_init_from_bi(bi_t **bi_p, bi_t *bi_copy);

/**
 * initializes big int from signed integer
 */
int bi_init_from_int(bi_t **bi_p, int num);

/**
 * initializes big int from string of [-0123456789]
 * returns RC from returncode.h or 6 if only [-] passed
 */
int bi_init_from_str(bi_t **bi_p, char *str);

#endif
