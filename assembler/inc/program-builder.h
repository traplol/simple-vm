#ifndef _PROJECTS_SIMPLE_VM_ASSEMBLER_PROGRAM_BUILDER_H
#define _PROJECTS_SIMPLE_VM_ASSEMBLER_PROGRAM_BUILDER_H

#include "token-list.h"

typedef enum {
    SYM_LABEL
} symbol_type_t;

typedef enum { DATA, TEXT } section_t;

typedef struct symbol {
    symbol_type_t type;
    section_t section;
    char *name;
    unsigned int address;
} symbol_t;

typedef struct program_info {
    size_t data_len;
    size_t data_idx;
    unsigned char *data;

    size_t text_len;
    size_t text_idx;
    unsigned char *text;

    section_t current_section;

    size_t sym_table_len;
    symbol_t **sym_table;
} program_info_t;


program_info_t *make_program_info(size_t sym_table_len);
void free_program_info(program_info_t **prog_info);
void generate_symbols(program_info_t *prog_info, token_list_t *tlist);
int compile_token(program_info_t *prog_data, token_t **token);


#endif

