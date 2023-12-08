#ifndef STACK_H
#define STACK_H

/*
 * ll_mexp_t - linked list math expression type
 * (big int | operation)
 */
typedef struct ll_mexp_s {
    struct ll_mexp_s *next;
    int data;
} ll_mexp_t;

ll_mexp_t *ll_mexp_push(ll_mexp_t *head, int value);

#endif