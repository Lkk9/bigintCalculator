#include "bigint.h"
#include "returncode.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ll_sd_push(ll_sd_t **head, sd_t *sd) {
    ll_sd_t *new_node = malloc(sizeof(ll_sd_t));
    if (new_node == NULL) {
        return RC_MEM_ERR;
    }
    new_node->digit = sd;
    new_node->next = *head;
    *head = new_node;
    return 0;
}

int sd_get_from_char(sd_t *sd, char digit, number_size index) {
    sd->data = ((digit / 2) << 1) | (digit % 2);
    sd->index = index;
    return 0;
}

int bi_str_to_ll_sd(ll_sd_t *head, char *str) {
    if (head != NULL) {
        return -1;
    }

    for (number_size i = 0; i < strlen(str); i++) {
        char cur_digit = str[i];
        sd_t *cur_sd = (sd_t *)malloc(sizeof(sd_t));
        if (cur_sd == NULL) {
            return RC_MEM_ERR;
        }
        sd_get_from_char(cur_sd, cur_digit - '0', i);
        ll_sd_push(&head, cur_sd);
    }

    return 0;
}

int bi_from_str(bi_t *bi, char *str) {
    unsigned is_negative = *str == '-';
    if (is_negative)
        str++;

    // remove leading zeros
    while (*str == '0' && *str != '\0')
        str++;

    if (*str == '\0') {
        // got only minus :skull:
        return 6;
    }
    printf("her1e\n");

    // error if weird symbols
    for (int i = strlen(str) - 1; i >= 0; i--) {
        printf("%d:%c ", i, str[i]);
        if (str[i] < '0' || str[i] > '9') {
            printf("wrong: %d\n", str[i]);
            return RC_INV_INPUT;
        }
    }
    printf("\nher2e\n");

    bi = (bi_t *)malloc(sizeof(bi_t));
    if (bi == NULL) {
        return RC_MEM_ERR;
    }
    bi->is_negative = is_negative;
    bi->first_digit = NULL;
    printf("healjksdjkasdre\n");

    bi_str_sds(bi, "98247349982743984729874827348729834");
    free(bi);
    return 0;
}