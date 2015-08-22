#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "helpers.h"

char *parse_string(char **copy);

int is_part_of_ident(int c) {
    return c == '_' || isalnum(c);
}

token_list_t *tokenize(char *code) {
    int tk_line_num = 1, tk_internal = 0;
    char *tk_str = NULL;
    char *copy = strdup(code);
    // note freeable_copy points to the beginning of copy while the copy pointer
    // is modified.
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
            for (; is_part_of_ident(*copy); ++copy);
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
        /* Directive */
        else if (*copy == '.' && isalpha(*(copy+1))) {
            pos = copy;
            ++copy;
            for (; is_part_of_ident(*copy); ++copy);
            tk_str = substr(pos, copy-pos);
            tk_type = TK_DIRECTIVE;
        }
        /* Char */
        else if (*copy == '\'' && isalpha(*(copy+1)) && *(copy+2) == '\'') {
            /* TODO: Escape codes. */
            ++copy; /* eat first ' */
            tk_str = substr(copy, 1);
            tk_type = TK_CHAR_LIT;
            copy += 2; /* eat second ' */
        }
        /* String */
        else if (*copy == '"') {
            tk_str = parse_string(&copy);
            tk_type = TK_STRING_LIT;
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
    push_back_token(tk_list, "EOF", -1, TK_EOF, 0);
    return tk_list;
}

char escape(char c) {
    switch (c) {
        default: return c;
        case 't': return '\t';
        case 'n': return '\n';
        case 'r': return '\r';
    }
}

/* Parses a "str\"ing" */
char *parse_string(char **copy) {
    #define SIZE 1024
    char *string, *old, *_copy, tmp[SIZE];
    int i = 0;
    _copy = *copy;
    if (*_copy != '"') {
        return NULL;
    }
    ++_copy;
    string = calloc(SIZE, sizeof(*string));
    while(1) {
        if (i + 1 >= SIZE) {
            tmp[i] = 0;
            old = string;
            string = str_cat(2, string, tmp);
            free(old);
            i = 0;
        }
        if (*_copy == '"') {
            tmp[i] = 0;
            old = string;
            string = str_cat(2, string, tmp);
            free(old);
            break;
        }
        else if (*_copy == '\\') {
            _copy++; /* eat the '\' */
            tmp[i++] = escape(*_copy);
        }
        else {
            tmp[i++] = *_copy;
        }
        _copy++;
    }
    #undef SIZE
    *copy = _copy+1;
    return string;
}
