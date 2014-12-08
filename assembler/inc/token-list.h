#ifndef _PROJECTS_SIMPLE_VM_ASSEMBLER_PARSER_H
#define _PROJECTS_SIMPLE_VM_ASSEMBLER_PARSER_H

#include "opdefs.h"

typedef enum {
    TK_UNKNOWN = 0,     /* Unknown token. */
    TK_PLUS,            /* '+' */
    TK_MINUS,           /* '-' */
    TK_STAR,            /* '*' */
    TK_FSLASH,          /* '/' */
    TK_COMMA,           /* ',' */
    TK_DOT,             /* '.' */
    TK_COLON,           /* ':' */
    TK_DOLLAR,          /* '$' */
    TK_LPAREN,          /* '(' */
    TK_RPAREN,          /* ')' */
    TK_IDENT,           /* Identifier */
    TK_INT_LIT,         /* Integer literal */
    TK_FLOAT_LIT,       /* Floating point literal */
    TK_STRING_LIT,      /* String literal */
    TK_CHAR_LIT,        /* Character literal */
} token_type_t;

typedef struct token {
    struct token *prev;
    token_type_t type;
    int line_num;
    char *str;
    union {
        opcode_t op;
        register_t reg;
        int imm;
    } internal;
    struct token *next;
} token_t;

typedef struct token_list {
    token_t *head, *tail;
} token_list_t;

token_list_t *make_token_list(void);
void free_token_list(token_list_t **list);

token_t *make_token(char *str, int line_num, token_type_t type, int internal);
void free_token(token_t **token);

token_t *push_back_token(token_list_t *list, char *str, int line_num, token_type_t type, int internal);

#endif

