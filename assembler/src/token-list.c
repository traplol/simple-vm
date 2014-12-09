#include <stdlib.h>

#include "token-list.h"
#include "helpers.h"
#include "instruction.h"

token_list_t *make_token_list(void) {
    token_list_t *list = malloc(sizeof *list);
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}
void free_token_list(token_list_t **list) {
    if (list && *list) {
        token_t *cur, *next;
        cur = (*list)->head;
        while (cur) {
            next = cur->next;
            free_token(&cur);
            cur = next;
        }
        free(*list);
        *list = NULL;
    }
}

token_t *make_token(char *str, int line_num, token_type_t type, int internal) {
    token_t *token = malloc(sizeof *token);
    token->prev = NULL;
    token->line_num = line_num;
    token->type = type;
    token->str = strdup(str);
    token->internal.imm = internal;
    token->next = NULL;
    return token;
}
void free_token(token_t **token) {
    if (token && *token) {
        free((*token)->str);
        free(*token);
        token = NULL;
    }
}

token_t *push_back_token(token_list_t *list, char *str, int line_num, token_type_t type, int internal) {
    token_t *t = make_token(str, line_num, type, internal);
    list->length++;
    if (list->head == NULL) {
        list->head = t;
        list->tail = t;
    }
    else {
        list->tail->next = t;
        t->prev = list->tail;
        list->tail = t;
    }
    return t;
}

