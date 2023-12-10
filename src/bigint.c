#include "bigint.h"
#include "returncode.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void ll_bi_show(bi_t *bi) {
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
    printf("\n(%llu) digits\n", bi->total_digits);
}

int bi_compare(bi_t *bi1, bi_t *bi2) {
    if (bi1->total_digits != bi2->total_digits) {
        return bi1->total_digits < bi2->total_digits ? -1 : 1;
    }
    ll_bi_t *bi1_cur_digit = bi1->first_digit;
    ll_bi_t *bi2_cur_digit = bi2->first_digit;
    for (uint64_t i = 0; i < bi1->total_digits; i++) {
        if (bi1_cur_digit->digit < bi2_cur_digit->digit) {
            return -1;
        } else if (bi1_cur_digit->digit > bi2_cur_digit->digit) {
            return 1;
        }
        bi1_cur_digit = bi1_cur_digit->next_digit;
        bi2_cur_digit = bi2_cur_digit->next_digit;
    }
    return 0;
}

int bi_add(bi_t *result, bi_t *addend1, bi_t *addend2) {
    int compare = bi_compare(addend1, addend2);
    ll_bi_show(addend1);
    ll_bi_show(addend2);
    if (compare == 1) {
        bi_t *tmp = addend2;
        addend2 = addend1;
        addend1 = tmp;
    }
    ll_bi_show(addend1);
    ll_bi_show(addend2);
    return 0;
}

void bi_free(bi_t **bi) {
    ll_bi_t *current = (*bi)->first_digit;
    ll_bi_t *previos = current;
    while (current != NULL) {
        previos = current;
        current = current->next_digit;
        free(previos);
    }
    free(*bi);
    *bi = NULL;
}

int ll_bi_push(bi_t *bi, digit_base_t digit) {
    ll_bi_t *ll_bi_new_digit = (ll_bi_t *)malloc(sizeof(ll_bi_t));
    if (ll_bi_new_digit == NULL) {
        return RC_MEM_ERR;
    }

    ll_bi_new_digit->digit = digit;
    bi->total_digits++;

    if (bi->first_digit == NULL) {
        ll_bi_new_digit->next_digit = NULL;
        ll_bi_new_digit->prev_digit = NULL;
        bi->first_digit = ll_bi_new_digit;
        bi->last_digit = ll_bi_new_digit;
    } else {
        ll_bi_new_digit->next_digit = bi->first_digit;
        bi->first_digit->prev_digit = ll_bi_new_digit;
        ll_bi_new_digit->prev_digit = NULL;
        bi->first_digit = ll_bi_new_digit;
    }

    return 0;
}

int bi_from_str(bi_t *bi, char *str) {
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
            printf("wrong, END: %d\n", str[i]);
            return RC_INV_INPUT;
        }
    }

    bi = (bi_t *)malloc(sizeof(bi_t));
    if (bi == NULL) {
        return RC_MEM_ERR;
    }
    bi->is_negative = is_negative;
    bi->first_digit = NULL;

    size_t str_len = strlen(str);
    size_t str_len_offset = 0;
    char quotient[str_len];
    for (size_t i = 0; i < str_len; i++) {
        quotient[i] = str[i] - '0';
    }

    uint8_t is_zero = 1;
    digit_base_t bits_count = 0;
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
            if (ll_bi_push(bi, bits) == RC_MEM_ERR) {
                bi_free(&bi);
                return RC_MEM_ERR;
            }
            bits = 0;
        } else if (is_zero) {
            if (ll_bi_push(bi, bits) == RC_MEM_ERR) {
                bi_free(&bi);
                return RC_MEM_ERR;
            }
        }

    } while (!is_zero);

    printf("calculation done!, result: \n");
    ll_bi_show(bi);
    return 0;
}