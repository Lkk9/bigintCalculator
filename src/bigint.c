#include "bigint.h"
#include "returncode.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void bi_show(bi_t *bi) {
    if (bi->is_negative) {
        printf("-");
    }

    ll_bi_t *current = bi->first_digit;
    while (current != NULL) {
        for (digit_base_t i = 0; i < DIGIT_BASE_SIZE; i++) {
            if (!(current->digit & (((digit_base_t)1 << (DIGIT_BASE_SIZE - 1)) >> i))) {
                printf("0");
            } else
                printf("1");
        }
        printf(" ");
        current = current->next_digit;
    }
    printf("\n");
    printf("(%llu) digits\n", bi->total_digits);
    printf("(%llu) ones\n", bi->ones);
}

int bi_compare(bi_t *bi1, bi_t *bi2, unsigned do_abs) {
    if (bi1->is_negative ^ bi2->is_negative) {
        return bi2->is_negative - bi1->is_negative;
    }
    uint8_t flip = !do_abs && bi1->is_negative && bi2->is_negative;
    if (bi1->total_digits != bi2->total_digits) {
        return bi1->total_digits < bi2->total_digits ? (flip ? 1 : -1) : (flip ? -1 : 1);
    }
    ll_bi_t *bi1_cur_digit = bi1->first_digit;
    ll_bi_t *bi2_cur_digit = bi2->first_digit;
    for (uint64_t i = 0; i < bi1->total_digits; i++) {
        if (bi1_cur_digit->digit < bi2_cur_digit->digit) {
            return flip ? 1 : -1;
        } else if (bi1_cur_digit->digit > bi2_cur_digit->digit) {
            return flip ? -1 : 1;
        }
        bi1_cur_digit = bi1_cur_digit->next_digit;
        bi2_cur_digit = bi2_cur_digit->next_digit;
    }
    return 0;
}

int bi_print(bi_t *bi, digit_base_t base) {
    bi_t *bi_in, *bi_in_next, *bi_zero, *bi_one, *bi_base, *bi_out;
    if (bi_init_from_bi(&bi_in, bi) == RC_ERR) {
        return RC_ERR;
    }
    if (bi_init_from_int(&bi_zero, 0) == RC_ERR) {
        bi_free(&bi_in);
        return RC_ERR;
    }
    if (bi_init_from_int(&bi_one, 1) == RC_ERR) {
        bi_free(&bi_in);
        bi_free(&bi_zero);
        return RC_ERR;
    }
    if (bi_init_from_int(&bi_base, base) == RC_ERR) {
        bi_free(&bi_in);
        bi_free(&bi_zero);
        bi_free(&bi_one);
        return RC_ERR;
    }
    if (bi_init(&bi_out) == RC_ERR) {
        bi_free(&bi_in);
        bi_free(&bi_zero);
        bi_free(&bi_base);
        bi_free(&bi_one);
        return RC_ERR;
    }
    if (bi_init(&bi_in_next) == RC_ERR) {
        bi_free(&bi_in);
        bi_free(&bi_zero);
        bi_free(&bi_base);
        bi_free(&bi_out);
        bi_free(&bi_one);
        return RC_ERR;
    }
    while (bi_compare(bi_in, bi_zero, 1) != 0) {

        digit_base_t remainder = 0;
        uint64_t total_bits = sizeof(digit_base_t) * 8 * bi_in->total_digits;
        for (uint64_t i = 0; i < total_bits; i++) {

            int cur_bit = bi_get_bit(bi_in, total_bits - 1 - i);
            if (cur_bit == -1) {
                bi_free(&bi_in);
                bi_free(&bi_zero);
                bi_free(&bi_base);
                bi_free(&bi_out);
                bi_free(&bi_in_next);
                bi_free(&bi_one);
                return RC_ERR;
            }
            remainder = 2 * remainder + cur_bit;

            if (remainder >= base) {
                if (bi_shift(&bi_in_next, bi_in_next, 0) == RC_ERR) {
                    bi_free(&bi_in);
                    bi_free(&bi_zero);
                    bi_free(&bi_base);
                    bi_free(&bi_out);
                    bi_free(&bi_in_next);
                    bi_free(&bi_one);
                    return RC_ERR;
                }

                if (bi_add(&bi_in_next, bi_in_next, bi_one) == RC_ERR) {
                    bi_free(&bi_in);
                    bi_free(&bi_zero);
                    bi_free(&bi_base);
                    bi_free(&bi_out);
                    bi_free(&bi_in_next);
                    bi_free(&bi_one);
                    return RC_ERR;
                }

                remainder -= base;
            } else {
                if (bi_shift(&bi_in_next, bi_in_next, 0) == RC_ERR) {
                    bi_free(&bi_in);
                    bi_free(&bi_zero);
                    bi_free(&bi_base);
                    bi_free(&bi_out);
                    bi_free(&bi_in_next);
                    bi_free(&bi_one);
                    return RC_ERR;
                }
            }
        }

        if (bi_set_bi(&bi_in, bi_in_next) == RC_ERR) {
            bi_free(&bi_in);
            bi_free(&bi_zero);
            bi_free(&bi_base);
            bi_free(&bi_out);
            bi_free(&bi_in_next);
            bi_free(&bi_one);
            return RC_ERR;
        }
        bi_clear(&bi_in_next);

        if (ll_bi_push(bi_out, remainder, 0) == RC_ERR) {
            bi_free(&bi_in);
            bi_free(&bi_zero);
            bi_free(&bi_base);
            bi_free(&bi_out);
            bi_free(&bi_in_next);
            bi_free(&bi_one);
            return RC_ERR;
        }
    }

    ll_bi_t *current = bi_out->first_digit;
    while (current != NULL) {
        if (sizeof(digit_base_t) == 8) {
            printf("%d", current->digit);
        } else {
            printf("%i", current->digit);
        }
        current = current->next_digit;
    }
    printf("\n");
    bi_free(&bi_in);
    bi_free(&bi_zero);
    bi_free(&bi_base);
    bi_free(&bi_out);
    bi_free(&bi_in_next);
    bi_free(&bi_one);
    return RC_OK;
}

void bi_swap(bi_t **bi1_p, bi_t **bi2_p) {
    bi_t *tmp = *bi2_p;
    *bi2_p = *bi1_p;
    *bi1_p = tmp;
}

int bi_add(bi_t **result, bi_t *bi1, bi_t *bi2) {
    uint8_t is_negative = 0;

    if (bi1->is_negative && bi2->is_negative) {
        is_negative = 1;
    } else if (bi1->is_negative) {
        bi_t *bi1_neg;
        if (bi_init_from_bi(&bi1_neg, bi1) == RC_ERR) {
            return RC_ERR;
        }
        bi1_neg->is_negative = 0;
        int ret = bi_sub(result, bi2, bi1_neg);
        bi_free(&bi1_neg);
        return ret;
    } else if (bi2->is_negative) {
        bi_t *bi2_neg;
        if (bi_init_from_bi(&bi2_neg, bi2) == RC_ERR) {
            return RC_ERR;
        }
        bi2_neg->is_negative = 0;
        int ret = bi_sub(result, bi1, bi2_neg);
        bi_free(&bi2_neg);
        return ret;
    }

    bi_t *bi_sum;
    if (bi_init(&bi_sum) == RC_ERR) {
        return RC_ERR;
    }

    bi_sum->is_negative = is_negative;

    int compare = bi_compare(bi1, bi2, 1);

    if (compare == 1) {
        bi_swap(&bi1, &bi2);
    }
    digit_base_t cur_sum = 0;
    uint8_t carry = 0;
    uint8_t is_exceeded = 0;
    unsigned bits_to_count = sizeof(digit_base_t) * 8;
    ll_bi_t *cur_bi1_digit = bi1->last_digit;
    ll_bi_t *cur_bi2_digit = bi2->last_digit;
    for (uint64_t i = 0; i < bi2->total_digits; i++) {
        cur_sum = 0;
        for (unsigned j = 0; j < bits_to_count; j++) {
            unsigned bit_next = (is_exceeded ? 0 : (cur_bi1_digit->digit >> j) & 1) + ((cur_bi2_digit->digit >> j) & 1) + carry;
            carry = bit_next >> 1;
            cur_sum = cur_sum | ((bit_next & 1) << j);
        }
        ll_bi_push(bi_sum, cur_sum, 0);

        cur_bi2_digit = cur_bi2_digit->prev_digit;
        if (i + 1 < bi1->total_digits) {
            cur_bi1_digit = cur_bi1_digit->prev_digit;
        } else {
            is_exceeded = 1;
        }
    }

    if (carry) {
        ll_bi_push(bi_sum, 1, 0);
    }
    bi_free(result);
    (*result) = bi_sum;
    return RC_OK;
}

int bi_sub(bi_t **result, bi_t *bi1, bi_t *bi2) {
    if (!bi1->is_negative && bi2->is_negative) {
        bi_t *bi2_neg;
        if (bi_init_from_bi(&bi2_neg, bi2) == RC_ERR) {
            return RC_ERR;
        }
        bi2_neg->is_negative = 0;
        int ret = bi_add(result, bi1, bi2_neg);
        bi_free(&bi2_neg);
        return ret;
    } else if (bi1->is_negative && !bi2->is_negative) {
        bi_t *bi1_neg;
        if (bi_init_from_bi(&bi1_neg, bi1) == RC_ERR) {
            return RC_ERR;
        }
        bi1_neg->is_negative = 0;
        int ret = bi_add(result, bi2, bi1_neg);
        bi_free(&bi1_neg);
        return ret;
    }

    bi_t *bi_sub;
    if (bi_init(&bi_sub) == RC_ERR) {
        return RC_ERR;
    }

    int compare = bi_compare(bi1, bi2, 1);
    if (compare != 1) {
        bi_swap(&bi1, &bi2);
        bi_sub->is_negative = !bi1->is_negative;
    } else {
        bi_sub->is_negative = bi1->is_negative;
    }

    int8_t borrow = 0;
    uint8_t is_exceeded = 0;
    digit_base_t cur_sub = 0;
    unsigned bits_to_count = sizeof(digit_base_t) * 8;
    ll_bi_t *cur_bi1_digit = bi1->last_digit;
    ll_bi_t *cur_bi2_digit = bi2->last_digit;
    for (uint64_t i = 0; i < bi1->total_digits; i++) {
        cur_sub = 0;
        for (unsigned j = 0; j < bits_to_count; j++) {
            int8_t bit_next = ((cur_bi1_digit->digit >> j) & 1) - (is_exceeded ? 0 : (cur_bi2_digit->digit >> j) & 1) + borrow;
            borrow = bit_next >> 1;
            cur_sub = cur_sub | ((bit_next & 1) << j);
        }
        ll_bi_push(bi_sub, cur_sub, 0);

        cur_bi1_digit = cur_bi1_digit->prev_digit;
        if (i + 1 < bi2->total_digits) {
            cur_bi2_digit = cur_bi2_digit->prev_digit;
        } else {
            is_exceeded = 1;
        }
    }
    bi_free(result);
    (*result) = bi_sub;
    return RC_OK;
}

int bi_get_bit(bi_t *bi, uint64_t index) {
    uint64_t bits_per_digit = sizeof(digit_base_t) * 8;
    if (index > bi->total_digits * bits_per_digit) {
        return -1;
    }

    uint64_t digit_index = index / bits_per_digit;

    ll_bi_t *current = bi->last_digit;
    for (uint64_t i = 0; i < digit_index; i++) {
        current = current->prev_digit;
    }
    return (current->digit >> (index - digit_index * bits_per_digit)) & 1;
}

int bi_shift(bi_t **result, bi_t *bi, unsigned dir) {

    bi_t *bi_shifted;
    if (bi_init(&bi_shifted) == RC_ERR) {
        return RC_ERR;
    }

    uint64_t bits_per_digit = sizeof(digit_base_t) * 8;
    unsigned carry_bit = 0;
    ll_bi_t *current = dir ? bi->first_digit : bi->last_digit;
    if (current == NULL) {
        if (ll_bi_push(bi_shifted, 0, 0) == RC_ERR) {
            return RC_ERR;
        }
    }
    while (current != NULL) {
        digit_base_t cur_digit = current->digit;
        unsigned cur_carry_bit = dir ? cur_digit & 1 : (cur_digit >> (bits_per_digit - 1)) & 1;
        if (dir) {
            cur_digit >>= 1;
            cur_digit |= carry_bit << (bits_per_digit - 1);
            ll_bi_push(bi_shifted, cur_digit, 1); // TODO: CHEVK
            current = current->next_digit;
        } else {
            cur_digit <<= 1;
            cur_digit |= carry_bit;
            ll_bi_push(bi_shifted, cur_digit, 0);
            current = current->prev_digit;
        }
        carry_bit = cur_carry_bit;
    }

    if (carry_bit) {
        if (dir) {
            ll_bi_push(bi_shifted, 1 << (bits_per_digit - 1), 1);
        } else {
            ll_bi_push(bi_shifted, 1, 0);
        }
    }
    bi_free(result);
    (*result) = bi_shifted;
    return RC_OK;
}

int bi_mul(bi_t **result, bi_t *bi1, bi_t *bi2) {
    bi_t *bi_mul;
    if (bi_init(&bi_mul)) {
        return RC_ERR;
    }

    ll_bi_push(bi_mul, 0, 0);

    if (bi1->ones > bi2->ones) {
        bi_swap(&bi1, &bi2);
    }

    bi_t *bi2_offset;
    if (bi_init_from_bi(&bi2_offset, bi2) == RC_ERR) {
        return RC_ERR;
    }
    bi2_offset->is_negative = 0;
    for (uint64_t i = 0; i < bi1->total_digits * sizeof(digit_base_t) * 8; i++) {
        int cur_bit = bi_get_bit(bi1, i);

        if (cur_bit == -1) {
            fprintf(stderr, "Mul: index out of range\n");
            return RC_ERR;
        }

        if (cur_bit) {
            bi_add(&bi_mul, bi_mul, bi2_offset);
        }
        bi_shift(&bi2_offset, bi2_offset, 0);
    }

    bi_free(&bi2_offset);
    bi_mul->is_negative = bi1->is_negative ^ bi2->is_negative;

    bi_free(result);
    (*result) = bi_mul;
    return RC_OK;
}

void bi_free(bi_t **bi_p) {
    ll_bi_t *current = (*bi_p)->first_digit;
    ll_bi_t *previos = current;
    while (current != NULL) {
        previos = current;
        current = current->next_digit;
        free(previos);
    }
    free(*bi_p);
    *bi_p = NULL;
}

int ll_bi_push(bi_t *bi, digit_base_t digit, unsigned dest) {
    ll_bi_t *ll_bi_new_digit = (ll_bi_t *)malloc(sizeof(ll_bi_t));
    if (ll_bi_new_digit == NULL) {
        fprintf(stderr, "Memory error: can not allocate memory\n");
        return RC_ERR;
    }

    ll_bi_new_digit->digit = digit;
    bi->total_digits++;
    uint64_t ones = 0;
    for (uint64_t i = 0; i < sizeof(digit_base_t) * 8; i++) {
        ones += (digit >> i) & 1;
    }
    bi->ones += ones;

    if (bi->first_digit == NULL) {
        ll_bi_new_digit->next_digit = NULL;
        ll_bi_new_digit->prev_digit = NULL;
        bi->first_digit = ll_bi_new_digit;
        bi->last_digit = ll_bi_new_digit;
    } else {
        if (dest) {
            ll_bi_new_digit->next_digit = NULL;
            bi->last_digit->next_digit = ll_bi_new_digit;
            ll_bi_new_digit->prev_digit = bi->last_digit;
            bi->last_digit = ll_bi_new_digit;
        } else {
            ll_bi_new_digit->next_digit = bi->first_digit;
            bi->first_digit->prev_digit = ll_bi_new_digit;
            ll_bi_new_digit->prev_digit = NULL;
            bi->first_digit = ll_bi_new_digit;
        }
    }

    return RC_OK;
}

int bi_copy_digits(bi_t **bi_p, bi_t *bi_copy) {
    // printf("LESSGOOOOO: ----------------\n");
    ll_bi_t *current = bi_copy->last_digit;
    for (uint64_t i = 0; i < bi_copy->total_digits; i++) {
        if (current == NULL) {
            fprintf(stderr, "Init: can not initialize big int from big int\n");
            return RC_ERR;
        }
        digit_base_t cur_digit = current->digit;
        ll_bi_push(*bi_p, cur_digit, 0);
        // bi_show(*bi_p);
        current = current->prev_digit;
    }
    (*bi_p)->is_negative = bi_copy->is_negative;
    return RC_OK;
}

int bi_set_bi(bi_t **bi_p, bi_t *bi_copy) {
    bi_clear(bi_p);
    if (bi_copy_digits(bi_p, bi_copy) == RC_ERR) {
        return RC_ERR;
    }
    return RC_OK;
}

static void bi_reset(bi_t **bi_p) {
    (*bi_p)->first_digit = NULL;
    (*bi_p)->last_digit = NULL;
    (*bi_p)->total_digits = 0;
    (*bi_p)->is_negative = 0;
    (*bi_p)->ones = 0;
}

int bi_init(bi_t **bi_p) {
    *bi_p = (bi_t *)malloc(sizeof(bi_t));
    if (*bi_p == NULL) {
        fprintf(stderr, "Memory error: can not allocate memory\n");
        return RC_ERR;
    }

    bi_reset(bi_p);

    return RC_OK;
}

void bi_clear(bi_t **bi_p) {
    ll_bi_t *current = (*bi_p)->first_digit;
    ll_bi_t *previos = current;
    while (current != NULL) {
        previos = current;
        current = current->next_digit;
        free(previos);
    }
    bi_reset(bi_p);
}

int bi_init_from_bi(bi_t **bi_p, bi_t *bi_copy) {
    if (bi_init(bi_p) == RC_ERR) {
        return RC_ERR;
    }

    if (bi_copy_digits(bi_p, bi_copy) == RC_ERR) {
        return RC_ERR;
    }
    // printf("HEREEREEEERR________)()()()\n");
    // bi_show(*bi_p);
    return RC_OK;
}

int bi_init_from_int(bi_t **bi_p, int num) {
    if (bi_init(bi_p) == RC_ERR) {
        return RC_ERR;
    }
    (*bi_p)->is_negative = num < 0;

    uint8_t splits = sizeof(int) / sizeof(digit_base_t);
    for (uint8_t i = 0; i < splits; i++) {
        ll_bi_push(*bi_p, (digit_base_t)num, 0);
        num >>= 8;
    }

    return RC_OK;
}

int bi_init_from_str(bi_t **bi_p, char *str) {
    unsigned is_negative = *str == '-';
    if (is_negative)
        str++;

    // remove leading zeros
    while (*str == '0' && *(str + 1) != '\0')
        str++;

    if (*str == '\0') {
        return 6;
    }

    // error if weird symbols
    for (int i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] < '0' || str[i] > '9') {
            fprintf(stderr, "wrong, END: %d\n", str[i]); // TODO: make good
            return RC_INV_INPUT;
        }
    }

    if (bi_init(bi_p) == RC_ERR) {
        return RC_ERR;
    }

    (*bi_p)->is_negative = is_negative;
    /* TODO: optimize
        // ll_bi_push(*bi_p, 0, 0);

        // bi_t *bi_base;
        // if (bi_init_from_int(&bi_base, 10) == RC_ERR) {
        //     bi_free(bi_p);
        //     return RC_ERR;
        // }

        // for (int i = 0; i < strlen(str); i++) {
        //     bi_t *bi_cur_digit;
        //     if (bi_init_from_int(&bi_cur_digit, str[i] - '0')) {
        //         bi_free(bi_p);
        //         bi_free(&bi_base);
        //         return RC_ERR;
        //     }

        //     bi_mul(bi_p, *bi_p, bi_base);
        //     bi_add(bi_p, *bi_p, bi_cur_digit);

        //     bi_free(&bi_cur_digit);
        // }
        // bi_free(&bi_base);
    */
    size_t str_len = strlen(str);
    size_t str_len_offset = 0;
    char quotient[str_len];
    for (size_t i = 0; i < str_len; i++) {
        quotient[i] = str[i] - '0';
    }

    uint8_t is_zero = 1;
    unsigned bits_count = 0;
    digit_base_t bits = 0;
    do {
        is_zero = 1;
        digit_base_t carry = 0;

        for (size_t i = str_len_offset; i < str_len; i++) {
            uint8_t cur_digit = quotient[i] + 10 * carry;
            quotient[i] = cur_digit >> 1;
            if (quotient[i] == 0 && is_zero) {
                str_len_offset++;
            }
            carry = (cur_digit & 1) ? 1 : 0;
            if (quotient[i] != 0)
                is_zero = 0;
        }

        bits = bits | (carry << (bits_count));
        bits_count++;

        if (bits_count == DIGIT_BASE_SIZE) {
            bits_count = 0;
            if (ll_bi_push((*bi_p), bits, 0) == RC_ERR) {
                bi_free(bi_p);
                return RC_ERR;
            }
            bits = 0;
        } else if (is_zero) {
            if (ll_bi_push((*bi_p), bits, 0) == RC_ERR) {
                bi_free(bi_p);
                return RC_ERR;
            }
        }

    } while (!is_zero);

    return 0;
}
