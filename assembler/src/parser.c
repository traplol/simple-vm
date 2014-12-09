#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "token-list.h"
#include "parser.h"

int expression(char **expr);

int number(char **expr) {
    int result = *((*expr)++) - '0';
    while (*(*expr) >= '0' && *(*expr) <= '9') {
        result = 10 * result + *((*expr)++) - '0';
    }
    return result;
}

int factor(char **expr) {
    if (*(*expr) >= '0' && *(*expr) <= '9') {
        return number(expr);
    }
    else if (*(*expr) == '(') {
        (*expr)++; /* '(' */
        int result = expression(expr);
        (*expr)++; /* ')' */
        return result;
    }
    else if (*((*expr)) == '-') {
        (*expr)++; /* '-' */
        return -expression(expr);
    }
    return 0;
}

int term(char **expr) {
    int result = factor(expr);
    while (*(*expr) == '*' || *(*expr) == '/') {
        if (*((*expr)++) == '*') {
            result *= factor(expr);
        }
        else {
            result /= factor(expr);
        }
    }
    return result;
}

int expression(char **expr) {
    int result = term(expr);
    while (*((*expr)) == '+' || *(*expr) == '-') {
        if (*((*expr)++) == '+') {
            result += term(expr);
        }
        else {
            result -= term(expr);
        }
    }
    return result;
}

int eval(char *expr) {
    return expression(&expr);
}

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
    int evaled = eval(catted_str);
    char buf[30];
    sprintf(buf, "%d", evaled);
    push_back_token(list, buf, st_line_num, TK_IMMEDIATE, evaled);
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
    /* New label */
    else if (tk->next && tk->next->type == TK_COLON) {
        push_back_token(list, tk->str, tk->line_num, TK_NEW_LABEL, 0);
        *tok = tk->next;
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
            default:
            case TK_UNKNOWN:
                fprintf(stderr, "Unknown token at line %d\n", tok->line_num);
                fprintf(stderr, "~~~~>%s\n", tok->str);
                free_token_list(&tk_list);
                return NULL;
            case TK_COMMA: break;
            case TK_IDENT: parse_ident(tk_list, &tok); break;
            case TK_LPAREN: parse_arithmetic_expr(tk_list, &tok); break;
            case TK_DOLLAR: parse_register(tk_list, &tok); break;
            case TK_DOT: parse_directive(tk_list, &tok); break;
        }
        tok = tok->next;
    }
    return tk_list;
}
