#include "stack.h"

#include <stdlib.h>

ll_mexp_t *ll_mexp_push(ll_mexp_t *head, int value) {
    ll_mexp_t *new_node = (ll_mexp_t *)malloc(sizeof(ll_mexp_t));
    new_node->next = head;
    new_node->data = value;
    return new_node;
}
