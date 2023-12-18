#include "mathexp.h"
#include "notation.h"
#include "returncode.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>

static token_t mexp_get_token_type(char *str) {
    size_t str_len = strlen(str);
    for (size_t i = 0; i < str_len; i++) {
        if ((str[i] < '0' || str[i] > '9') && !(i == 0 && str[0] == '-'))
            break;
        if (i + 1 == str_len && !(str_len == 1 && str[0] == '-'))
            return NUMBER;
    }
    if (str_len != 1)
        return OTHER;
    switch (str[0]) {
    case ' ':
        return SPACE;
    case '-':
        return MINUS;
    case '(':
        return PARENTHESIS_OPEN;
    case ')':
        return PARENTHESIS_CLOSE;
    case '*':
        return STAR;
    case '+':
        return PLUS;
    case '/':
        return SLASH;
    }

    return OTHER;
}

int mexp_parse(mexp_t *mexp, char *str, notation_t input_type) {
    char *token;
    char *sep = " ";
    switch (input_type) {
    case INFIX:
        // int parentheses = 0;
        fprintf(stderr, "Notation: unsupported notation type (infix)\n");
        return RC_NO_SUPPORT_INFIX;
        break;
    case REVPOL:
        token = strtok(str, sep);
        size_t i = 0;
        while (token != NULL) {
            i += strlen(token);
            token_t token_type = mexp_get_token_type(token);
            bi_t *num;
            operator_t op = NONE;
            switch (token_type) {
            case OTHER:
            case PARENTHESIS_CLOSE:
            case PARENTHESIS_OPEN:
                fprintf(stderr, "Input: invalid token type at %zu\n", i);
                return RC_INV_INPUT;
            case NUMBER:
                bi_init_from_str(&num, token);
                mexp_enqueue_number(mexp, num);
                break;
            case PLUS:
                op = ADD;
                break;
            case MINUS:
                op = SUB;
                break;
            case STAR:
                op = MUL;
                break;
            case SLASH:
                op = DIV;
                fprintf(stderr, "Division: unsupported operation\n");
                return RC_NO_SUPPORT_DIVISION;
                break;
            default:
                fprintf(stderr, "Operation: Unsupported operation\n");
            }

            if (op != NONE) {
                mexp_enqueue_operator(mexp, op);
            }
            token = strtok(NULL, sep);
        }

        break;
    }
    return RC_OK;
}

void mexp_show(mexp_t *mexp) {
    ll_mexp_t *current = mexp->first;
    while (current != NULL) {
        printf("NODE TYPE: %s\n", current->type == NUM ? "number" : "operation");
        if (current->type == NUM) {
            bi_show(current->number);
        } else {
            printf("\toperation: %d\n", current->op);
        }
        current = current->next;
    }
}

int mexp_enqueue_number(mexp_t *mexp, bi_t *number) {
    ll_mexp_t *new_node = (ll_mexp_t *)malloc(sizeof(ll_mexp_t));
    if (new_node == NULL) {
        return RC_ERR;
    }
    new_node->type = NUM;
    new_node->number = number;
    new_node->next = NULL;
    if (mexp->last == NULL) {
        mexp->first = new_node;
        mexp->last = new_node;
    } else {
        mexp->last->next = new_node;
        mexp->last = new_node;
    }
    return RC_OK;
}

int mexp_push_number(mexp_t *mexp, bi_t *number) {
    ll_mexp_t *new_node = (ll_mexp_t *)malloc(sizeof(ll_mexp_t));
    if (new_node == NULL) {
        return RC_ERR;
    }
    new_node->type = NUM;
    new_node->number = number;
    if (mexp->last == NULL) {
        mexp->first = new_node;
        mexp->last = new_node;
        new_node->next = NULL;
    } else {
        new_node->next = mexp->first;
        mexp->first = new_node;
    }
    return RC_OK;
}

int mexp_enqueue_operator(mexp_t *mexp, operator_t op) {
    ll_mexp_t *new_node = (ll_mexp_t *)malloc(sizeof(ll_mexp_t));
    if (new_node == NULL) {
        return RC_ERR;
    }
    new_node->type = OPERATOR;
    new_node->number = NULL;
    new_node->op = op;
    new_node->next = NULL;
    if (mexp->last == NULL) {
        mexp->first = new_node;
        mexp->last = new_node;
    } else {
        mexp->last->next = new_node;
        mexp->last = new_node;
    }
    return RC_OK;
}

int mexp_push_operator(mexp_t *mexp, operator_t op) {
    ll_mexp_t *new_node = (ll_mexp_t *)malloc(sizeof(ll_mexp_t));
    if (new_node == NULL) {
        return RC_ERR;
    }
    new_node->type = OPERATOR;
    new_node->number = NULL;
    new_node->op = op;
    if (mexp->last == NULL) {
        mexp->first = new_node;
        mexp->last = new_node;
        new_node->next = NULL;
    } else {
        new_node->next = mexp->first;
        mexp->first = new_node;
    }
    return RC_OK;
}

void mexp_free(mexp_t **mexp) {
    ll_mexp_t *current = (*mexp)->first;
    while (current != NULL) {
        if (current->type == NUM) {
            bi_free(&(current->number));
        }
        ll_mexp_t *tmp = current;
        current = current->next;
        free(tmp);
    }
    free(*mexp);
    *mexp = NULL;
}

int mexp_pop(mexp_t *mexp) {

    if (mexp->first == NULL) {
        return -1;
    }

    if (mexp->first->type == NUM) {
        bi_free(&(mexp->first->number));
    }
    ll_mexp_t *tmp = mexp->first;
    mexp->first = mexp->first->next;
    free(tmp);
    if (mexp->first == NULL) {
        mexp->last = NULL;
    }
    return RC_OK;
}

int mexp_init(mexp_t **mexp) {
    *mexp = (mexp_t *)malloc(sizeof(mexp_t));
    if (*mexp == NULL) {
        return RC_ERR;
    }

    (*mexp)->first = NULL;
    (*mexp)->last = NULL;
    return RC_OK;
}

static size_t mexp_get_size(mexp_t *mexp) {
    size_t i = 0;
    ll_mexp_t *current = mexp->first;
    while (current != NULL) {
        current = current->next;
        i++;
    }
    return i;
}

int mexp_calculate(mexp_t *mexp, bi_t **result) {
    mexp_t *mexp_calc;
    if (mexp_init(&mexp_calc) == RC_ERR) {
        return RC_ERR;
    }

    ll_mexp_t *current_item = mexp->first;
    while (current_item != NULL) {
        if (current_item->type == OPERATOR) {

            if (mexp_calc->first == NULL || mexp_calc->first->next == NULL) {
                fprintf(stderr, "Math Expression: incorrect format for the notation\n");
                return RC_INV_INPUT;
            }
            bi_t *number;
            if (bi_init(&number) == RC_ERR) {
                return RC_ERR;
            }
            switch (current_item->op) {
            case ADD:

                if (bi_add(&number, mexp_calc->first->number, mexp_calc->first->next->number) == RC_ERR) {
                    return RC_ERR;
                }

                break;
            case SUB:
                if (bi_sub(&number, mexp_calc->first->number, mexp_calc->first->next->number) == RC_ERR) {
                    return RC_ERR;
                }
                break;
            case MUL:
                if (bi_mul(&number, mexp_calc->first->number, mexp_calc->first->next->number) == RC_ERR) {
                    return RC_ERR;
                }
                break;
            case DIV:
                break;
            case NONE:
                fprintf(stderr, "Operation: Met None operation\n");
            }

            mexp_pop(mexp_calc);
            mexp_pop(mexp_calc);
            if (mexp_push_number(mexp_calc, number) == RC_ERR) {
                return RC_ERR;
            }

        } else if (current_item->type == NUM) {
            bi_t *number;

            if (bi_init_from_bi(&number, current_item->number) == RC_ERR) {
                return RC_ERR;
            }
            if (mexp_push_number(mexp_calc, number) == RC_ERR) {
                return RC_ERR;
            }
        }

        current_item = current_item->next;
    }

    if (mexp_get_size(mexp_calc) == 1) {
        bi_init_from_bi(result, mexp_calc->first->number);
    }
    mexp_free(&mexp_calc);

    return RC_OK;
}
