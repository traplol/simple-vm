#ifndef SIMPLE_VM_HELPERS_H
#define SIMPLE_VM_HELPERS_H
#include "defs.h"

/* Returns the opcode as a readable string. */
char *op_to_str(opcode_t op);
/* Returns the register as a readable string. */
char *reg_to_str(register_t r);
/* Returns the immediate as a readable string.
 * Note: This needs to be freed. */
char *imm_to_str(signed int imm, char *fmt);


/* Returns a new string with all of the strings passes concatenated. 
 * Note: This needs to be freed. */
char *str_cat(unsigned long count, ...);

void assert(int cond, char *msg);

#endif

