#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "program-builder.h"
#include "instruction.h"
#include "helpers.h"
#include "vm.h"

void err(char *msg, int line_num) {
    fprintf(stderr, "Error: %s at line %d\n", msg, line_num);
}

program_info_t *make_program_info(size_t sym_table_len) {
    program_info_t *prog_info = malloc(sizeof *prog_info);

    prog_info->data_idx = 0;
    prog_info->data_len = 128;
    prog_info->data = malloc(prog_info->data_len * sizeof *(prog_info->data));

    prog_info->text_idx = 0;
    prog_info->text_len = 128;
    prog_info->text = malloc(prog_info->text_len * sizeof *(prog_info->text));
    prog_info->current_section = TEXT;

    prog_info->sym_table_len = sym_table_len;
    prog_info->sym_table = calloc(sym_table_len, sizeof *(prog_info->sym_table));

    return prog_info;
}

void free_program_info(program_info_t **prog_info) {
    if (prog_info && *prog_info) {
        free((*prog_info)->data);
        free((*prog_info)->text);
        for (size_t i = 0; i < (*prog_info)->sym_table_len; ++i) {
            if ((*prog_info)->sym_table[i]) {
                free((*prog_info)->sym_table[i]->name);
                free((*prog_info)->sym_table[i]);
            }
        }
        free((*prog_info)->sym_table);
        free(*prog_info);
        *prog_info = NULL;
    }
}

size_t hash(char *str) {
    size_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

symbol_t *lookup_symbol(symbol_t **sym_table, size_t len, char *str) {
    size_t i;
    size_t h_idx = hash(str) % len;
    if (sym_table[h_idx]) {
        if (strcmp(sym_table[h_idx]->name, str) == 0) {
            return sym_table[h_idx];
        }
    }
    for (i = h_idx; i < len; ++i) {
        if (sym_table[i]) {
            if (strcmp(sym_table[i]->name, str) == 0) {
                return sym_table[i];
            }
        }
    }
    for (i = 0; i < h_idx; ++i) {
        if (sym_table[i]) {
            if (strcmp(sym_table[i]->name, str) == 0) {
                return sym_table[i];
            }
        }
    }
    return NULL;
}

int get_section_offset(program_info_t *prog_info) {
    if (prog_info->current_section == TEXT) {
        return TEXT_SECTION_START;
    }
    if (prog_info->current_section == DATA) {
        return DATA_SECTION_START;
    }
    return 0;
}

symbol_t *insert_symbol(program_info_t *prog_info, token_t *token) {
    symbol_t *symbol;
    size_t len = prog_info->sym_table_len;
    size_t index = hash(token->str) % len;
    /* Symbol already in table. */
    if ((symbol = lookup_symbol(prog_info->sym_table, prog_info->sym_table_len, token->str))) { 
        return symbol;
    }
    symbol = malloc(sizeof *symbol);
    symbol->section = prog_info->current_section;
    symbol->name = strdup(token->str);
    if (prog_info->current_section == TEXT) {
        symbol->address = prog_info->text_idx + get_section_offset(prog_info);
    }
    else if (prog_info->current_section == DATA) {
        symbol->address = prog_info->data_idx + get_section_offset(prog_info);
    }
    /* These two for loops allow for a single full loop over 
     * the hashtable starting at the generated hash index. */
    for (size_t i = index; i < prog_info->data_len; ++i) {
        if (prog_info->sym_table[i] == NULL) {
            prog_info->sym_table[i] = symbol;
            return symbol;
        }
    }
    for (size_t i = 0; i < index; ++i) {
        if (prog_info->sym_table[i] == NULL) {
            prog_info->sym_table[i] = symbol;
            return symbol;
        }
    }
    return NULL;
}

int lookup_label_offset(program_info_t *prog_info, opcode_t opcode, token_t *token, int *passfail) {
    symbol_t *symbol = lookup_symbol(prog_info->sym_table, prog_info->sym_table_len, token->str);
    int offset = -1;
    if (symbol == NULL) {
        err("Label reference not found", token->line_num);
    }
    else if (is_pc_relative(opcode)) {
        if (prog_info->current_section == DATA) {
            *passfail = 0;
            return symbol->address - prog_info->data_idx - DATA_SECTION_START;
        }
        else if (prog_info->current_section == TEXT) {
            *passfail = 0;
            return symbol->address - prog_info->text_idx - TEXT_SECTION_START;
        }
    }
    else {
        *passfail = 0;
        return symbol->address;
    }
    *passfail = -1;
    return offset;
}


void insert_text_or_data(program_info_t *prog_info, void *thing, size_t thing_size) {
    if (prog_info->current_section == DATA) {
        /* Resize the data section. */
        if (prog_info->data_idx + thing_size > prog_info->data_len) {
            prog_info->data_len *= 2;
            prog_info->data = realloc(prog_info->data, prog_info->data_len);
        }

        memcpy(&prog_info->data[prog_info->data_idx], thing, thing_size);
        /* DS is single bytes so we can just increase the index by the size. */
        prog_info->data_idx += thing_size;
    }
    else if (prog_info->current_section == TEXT) {
        /* Resize the text section. */
        if (prog_info->text_idx  >= prog_info->text_len ) {
            prog_info->text_len *= 2;
            prog_info->text = realloc(prog_info->text, prog_info->text_len * sizeof *(prog_info->text));
        }
        memcpy(&prog_info->text[prog_info->text_idx], thing, thing_size);
        /* thing_size is a multiple of the size of each instruction */
        prog_info->text_idx += thing_size;
    }
}

int compile_directive(program_info_t *prog_info, token_t **token) {
    token_t *tk = *token;
    if (strcmp(tk->str, "data") == 0) { prog_info->current_section = DATA; }
    else if (strcmp(tk->str, "text") == 0) { prog_info->current_section = TEXT; }
    else {
        err("Unknown directive", (*token)->line_num);
        return -1;
    }

    /* TODO: .word, .ascii */
    *token = (*token)->next;
    return 0;
}

int compile_instruction(program_info_t *prog_info, token_t **token) {
    token_t *tk = *token;
    int num_operands = get_num_operands(tk->internal.op);
    instruction_t *instr;
    opcode_t op = tk->internal.op;
    int operand1, operand2, passfail = 0;
    switch (num_operands) {
        case 0:
            tk = tk->next;
            instr = make_instruction(op, 0, 0);
            break;
        case 1:
            tk = tk->next;
            if (tk->type == TK_LABEL_REF) {
                operand1 = lookup_label_offset(prog_info, op, tk, &passfail);
            }
            else {
                operand1 = tk->internal.imm;
            }
            instr = make_instruction(op, operand1, 0);
            tk = tk->next;
            break;
        case 2:
            tk = tk->next;
            operand1 = tk->internal.imm;
            tk = tk->next;
            if (tk->type == TK_LABEL_REF) {
                operand2 = lookup_label_offset(prog_info, op, tk, &passfail);
            }
            else {
                operand2 = tk->internal.imm;
            }
            instr = make_instruction(op, operand1, operand2);
            tk = tk->next;
            break;
    }
    puts(instr->disassembled_str);
    *token = tk;

    if (passfail == 0) {
        passfail = instr ? 0 : -1;
        if (passfail == 0) {
            insert_text_or_data(prog_info, &(instr->assembled_value), sizeof (instr->assembled_value));
        }
    }
    free_instruction(&instr);
    return passfail;
}

int compile_new_label(program_info_t *prog_info, token_t **token) {
    token_t *tk = *token;
    if (lookup_symbol(prog_info->sym_table, prog_info->sym_table_len, (*token)->str)) {
        err("Label already defined", (*token)->line_num);
        return -1;
    }
    symbol_t *s = insert_symbol(prog_info, tk);
    *token = tk->next;
    return s->address;
}

void generate_symbols(program_info_t *prog_info, token_list_t *tlist) {
    token_t *cur = tlist->head;
    int num_operands;
    while (cur) {
        if (cur->type == TK_DIRECTIVE) {
            compile_directive(prog_info, &cur);
            continue;
        }
        else if (cur->type == TK_NEW_LABEL) {
            compile_new_label(prog_info, &cur);
            continue;
        }
        else if (cur->type == TK_INSTR) {
            num_operands = get_num_operands(cur->internal.op);
            switch (num_operands) {
                case 0:
                    break;
                case 1:
                    cur = cur->next;
                    break;
                case 2:
                    cur = cur->next->next;
                    break;
            }
            if (prog_info->current_section == TEXT) {
                prog_info->text_idx += 4;
            }
        }
        else if (prog_info->current_section == DATA) {
            /* TODO: Get size of data symbols. */
        }
        cur = cur->next;
    }
    prog_info->text_len = prog_info->text_idx;
    prog_info->text = realloc(prog_info->text, prog_info->text_len * sizeof *(prog_info->text));

    prog_info->data_len = prog_info->data_idx;
    prog_info->data = realloc(prog_info->data, prog_info->data_len);

    /* Reset indices */
    prog_info->text_idx = 0;
    prog_info->data_idx = 0;
    prog_info->current_section = TEXT;
}

int compile_token(program_info_t *prog_info, token_t **token) {
    switch ((*token)->type) {
        default:
            printf("~~~~>%s\n",(*token)->str);
            *token = (*token)->next;
            return -1;
        case TK_NEW_LABEL:
            *token = (*token)->next;
            return 0;
        case TK_DIRECTIVE:
            return compile_directive(prog_info, token);
        case TK_INSTR:
            return compile_instruction(prog_info, token);
    }
}



