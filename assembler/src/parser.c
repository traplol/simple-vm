#include <stdio.h>

#include "parser.h"
#include "helpers.h"

token_list_t *tlist;
token_t *cur_tok;

void parse_ident(void) {

}

void parse(token_list_t *list) {
    tlist = list;
    cur_tok = list->head;

    while (cur_tok) {
        switch (cur_tok->type) {
            case TK_UNKNOWN:
                fprintf(stderr, "Unknown token at line %d\n", cur_tok->line_num);
                return;
            case TK_IDENT: parse_ident(); break;
            case TK_LPAREN: parse_arithmetic_expr(); break;
            case TK_DOLLAR: parse_register(); break;

        }
    }
}

build_instruction() {
    token_t *cur = cur_tok;
    while (cur) {
        switch (cur->type) {
            case TK_UNKNOWN:
                return NULL;
            case TK_IDENT:
                parse

        }
        cur = cur->next;
    }
    cur_tok = cur;
}
