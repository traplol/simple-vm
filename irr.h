#ifndef SIMPLE_VM_IRR_H
#define SIMPLE_VM_IRR_H

#include "defs.h"

/* register to register */
/* oooooo rrrrr rrrrr 0000000000000000 */
/*    6     5     5         16         */
typedef struct irr {
    opcode_t op;
    register_t r1, r2;
    char *op_str, *r1_str, *r2_str, *compiled_str;
} irr_t;


irr_t *make_irr(opcode_t op, register_t r1, register_t r2);
void free_irr(irr_t **irr);
irr_t *decompile_irr(unsigned int ins);
unsigned int compile_irr(opcode_t op, register_t r1, register_t r2);

#endif

