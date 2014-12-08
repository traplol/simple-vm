#ifndef _PROJECTS_SIMPLE_VM_ASSEMBLER_PARSER_H
#define _PROJECTS_SIMPLE_VM_ASSEMBLER_PARSER_H

#include "token-list.h"
include "instruction.h"

void init_parser(token_list_t *list);
void denit_parser(void);

instruction_t *build_instruction();


#endif

