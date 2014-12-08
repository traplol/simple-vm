#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "helpers.h"

token_list_t *tokenize(char *code) {
    int tk_line_num = 0, tk_internal = 0;
    char *tk_str = NULL;
    char *copy = strdup(code);
    char *pos, *freeable_copy = copy;
    token_type_t tk_type;
    token_list_t *tk_list = make_token_list();
    while (*copy != 0) {
        /* Skip over white space. */
        if (isspace(*copy)) {
            if (*copy == '\n') {
                ++tk_line_num;
            }
            ++copy;
            continue;
        }
        /* Ident, [_a-zA-Z]+[_a-zA-Z0-9]* */
        if (*copy == '_' || isalpha(*copy)) {
            pos = copy;
            ++copy;
            for (; *copy == '_' || isalnum(*copy); ++copy);
            tk_str = substr(pos, copy-pos);
            tk_type = TK_IDENT;
        }
        /* Number literal */
        else if (isdigit(*copy)) {
            pos = copy;
            for (; isdigit(*copy); ++copy);
            tk_str = substr(pos, copy-pos);
            tk_type = TK_INT_LIT;
        }
        /* Comment */
        else if (*copy == '#') {
            do ++copy;
            while (*copy != 0 && *copy != '\n' && *copy != '\r');
            continue;
        }
        else {
            switch (*copy) {
                default: tk_str = strdup("(unknown)"); tk_type = TK_UNKNOWN;
                         break;
                case '+': tk_str = strdup("+"); tk_type = TK_PLUS; break;
                case '-': tk_str = strdup("-"); tk_type = TK_MINUS; break;
                case '*': tk_str = strdup("*"); tk_type = TK_STAR; break;
                case '/': tk_str = strdup("/"); tk_type = TK_FSLASH; break;
                case ',': tk_str = strdup(","); tk_type = TK_COMMA; break;
                case '.': tk_str = strdup("."); tk_type = TK_DOT; break;
                case ':': tk_str = strdup(":"); tk_type = TK_COLON; break;
                case '$': tk_str = strdup("$"); tk_type = TK_DOLLAR; break;
                case '(': tk_str = strdup("("); tk_type = TK_LPAREN; break;
                case ')': tk_str = strdup(")"); tk_type = TK_RPAREN; break;
            }
            ++copy;
        }
        push_back_token(tk_list, tk_str, tk_line_num, tk_type, tk_internal);
        free(tk_str);
        tk_str = NULL;
    }
    free(freeable_copy);
    return tk_list;
}

