#ifndef BIGINT_H
#define BIGINT_H

typedef unsigned int digit_base;
typedef unsigned long long number_size;

typedef struct ll_bi_s {
    struct ll_bi_s *next_digit;
    digit_base digit;
} ll_bi_t;

typedef struct bi_s {
    ll_bi_t *first_digit;
    unsigned char is_negative;
} bi_t;

typedef struct sd_s {   // sd = split digit, bcz it splits digit in half
    number_size index;  // position of digit in number (count from right)
    unsigned char data; // 000(0000)(0) - last zero shows if that digit even (0) or odd (1) , 4 zeors before show digit after dividing and adding (0-9)
} sd_t;

typedef struct ll_sd_s {
    sd_t *digit;
    struct ll_sd_s *next;
} ll_sd_t;

/*
 */
int sd_get_from_char(sd_t *, char, number_size);

/* INPUT: string of char "0123456789" only
 */
int bi_str_sds(ll_sd_t *, char *);

/*
 * return values:
 * 0 if everything is ok
 * 4 if not [-0123456789]
 * 6 if only [-]
 * 5 if memory allocation failed
 */
int bi_from_str(bi_t *, char *);

#endif