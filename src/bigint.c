#include "bigint.h"
#include "returncode.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// int bi_compare(bi_t *bi1, bi_t *bi2) {}

// int bi_add(bi_t *result, bi_t *addend1, bi_t *addend2) { int compare = bi_compare(addend1, addend2); }

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
    ll_bi_new_digit->next_digit = bi->first_digit;
    ll_bi_new_digit->digit = digit;
    bi->first_digit = ll_bi_new_digit;
    bi->total_digits++;
    return 0;
}

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
        for (size_t i = 0; i < str_len; i++) {
            uint8_t cur_digit = quotient[i] + 10 * carry;
            quotient[i] = cur_digit / 2;
            carry = cur_digit % 2;
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