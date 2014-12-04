#ifndef SIMPLE_VM_IRM_H
#define SIMPLE_VM_IRM_H

#include "defs.h"

/* immediate to register */
/* oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm */
/*    6     5           21            */
typedef struct irm {
    opcode_t op;
    register_t r;
    signed int imm;
    char *op_str, *r_str, *imm_str, *compiled_str;
} irm_t;


irm_t *make_irm(opcode_t op, register_t r, signed int imm);
void free_irm(irm_t **irm);
irm_t *decompile_irm(unsigned int ins);

unsigned int compile_irm(opcode_t op, register_t r, signed int imm);

/* Tests that a compiled irm format decompiles back to the arguments passes. */
void test_irm(opcode_t test_op, register_t test_reg, signed int test_imm);
#endif

