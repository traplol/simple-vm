#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "token-list.h"
#include "parser.h"

void parse_arithmetic_expr(token_list_t *list, token_t **tok) {
    /* Parse arithmetic expression. */
    char *catted_str = NULL, *tmp;
    token_t *start = *tok;
    token_t *end = start->next;
    int st_line_num = start->line_num;
    int paren_count = 1;
    while (paren_count) {
        if (end == NULL) {
            fprintf(stderr, "Mismatched parens at line %d\n", start->line_num);
            return;
        }
        switch (end->type) {
            case TK_PLUS:
            case TK_MINUS:
            case TK_STAR:
            case TK_FSLASH:
            case TK_INT_LIT:
            case TK_FLOAT_LIT:
            case TK_CHAR_LIT:
                break;
            case TK_LPAREN:
                ++paren_count;
                break;
            case TK_RPAREN:
                --paren_count;
                break;
        }
        end = end->next;
    }

    while (start != end) {
        tmp = catted_str;
        if (!catted_str) {
            catted_str = str_cat(2, "", start->str);
        }
        else {
            catted_str = str_cat(2, catted_str, start->str);
        }
        free(tmp);
        start = start->next;
    }
    push_back_token(list, catted_str, st_line_num, TK_IMMEDIATE, 0);
    *tok = end->prev;
    free(catted_str);
}

void parse_ident(token_list_t *list, token_t **tok) {
    token_t *tk = *tok;
    opcode_t op = str_to_op(tk->str);
    /* Operation */
    if (op != NOT_AN_OPCODE) {
        push_back_token(list, tk->str, tk->line_num, TK_INSTR, op);
    }
    else if (tk->next) {
        /* New label */
        if (tk->next->type == TK_COLON) {
            push_back_token(list, tk->str, tk->line_num, TK_NEW_LABEL, 0);
            *tok = tk->next;
        }
    }
    /* Label reference. */
    else {
        push_back_token(list, tk->str, tk->line_num, TK_LABEL_REF, 0);
    }
}

void parse_register(token_list_t *list, token_t **tok) {
    token_t *tk = *tok;
    register_t r;
    if (tk->next) {
        tk = tk->next;
        r = str_to_reg(tk->str);
        /* Found a register. */
        if (r != NOT_A_REGISTER) {
            *tok = tk;
            push_back_token(list, tk->str, tk->line_num, TK_REGISTER, r);
            return;
        }
    }
    fprintf(stderr, "Expected register identifer at line %d\n", tk->line_num);
}

void parse_directive(token_list_t *list, token_t **tok) {
    token_t *tk = *tok;
    if (tk->next) {
        tk = tk->next;
        *tok = tk;
        push_back_token(list, tk->str, tk->line_num, TK_DIRECTIVE, 0);
    }
    else {
        fprintf(stderr, "Expected directive at line %d\n", tk->line_num);
    }
}

token_list_t *parse(token_list_t *list) {
    token_list_t *tk_list = make_token_list();
    token_t *tok = list->head;

    while (tok) {
        switch (tok->type) {
            case TK_UNKNOWN:
                fprintf(stderr, "Unknown token at line %d\n", tok->line_num);
                return NULL;
            case TK_IDENT: parse_ident(tk_list, &tok); break;
            case TK_LPAREN: parse_arithmetic_expr(tk_list, &tok); break;
            case TK_DOLLAR: parse_register(tk_list, &tok); break;
            case TK_DOT: parse_directive(tk_list, &tok); break;
        }
        tok = tok->next;
    }
    return tk_list;
}
