#include <stdlib.h> /* malloc, free */

#include "irr.h"
#include "helpers.h"


irr_t *make_irr(opcode_t op, register_t r1, register_t r2) {
    irr_t *irr = malloc(sizeof *irr);
    irr->op = op;
    irr->r1 = r1;
    irr->r2 = r2;
    irr->op_str = op_to_str(op);
    irr->r1_str = reg_to_str(r1);
    irr->r2_str = reg_to_str(r2);
    irr->compiled_str = str_cat(5, irr->op_str, " ", irr->r1_str, ", ", irr->r2_str);
    return irr;
}

void free_irr(irr_t **irr) {
    if (irr && *irr) {
        free((*irr)->compiled_str);
        free(*irr);
        *irr = NULL;
    }
}

irr_t *decompile_irr(unsigned int ins) {
    opcode_t op = ins & (OPCODE_MASK << OPCODE_SHIFT);
    op >>= OPCODE_SHIFT;

    register_t r1 = ins & (REGISTER_MASK << R1_SHIFT);
    r1 >>= R1_SHIFT;

    register_t r2 = ins & (REGISTER_MASK << R2_SHIFT);
    r2 >>= R2_SHIFT;

    irr_t *irr = make_irr(op, r1, r2);
    return irr;
}

/* register to register */
/* oooooo rrrrr rrrrr 0000000000000000 */
/*    6     5     5         16         */
unsigned int compile_irr(opcode_t op, register_t r1, register_t r2) {
    unsigned int num = 0;
    /* opcode */
    num |= (op << OPCODE_SHIFT);
    /* first register */
    num |= (r1 << R1_SHIFT);
    /* second register */
    num |= (r2 << R2_SHIFT);
    return num;
}

/* Tests that a compiled irr format decompiles back to the arguments passes. */
void test_irr(opcode_t test_op, register_t test_r1, register_t test_r2) {
    unsigned int test = compile_irr(test_op, test_r1, test_r2);
    irr_t *irr = decompile_irr(test);

    char *op_s, *r1_s, *r2_s;
    op_s = str_cat(4, "irr: ", irr->op_str, " != ", op_to_str(test_op));
    r1_s = str_cat(4, "irr: ", irr->r1_str, " != ", reg_to_str(test_r1));
    r2_s = str_cat(4, "irr: ", irr->r2_str, " != ", reg_to_str(test_r2));

    assert(irr->op == test_op, op_s);
    assert(irr->r1 == test_r1, r1_s);
    assert(irr->r2 == test_r2, r2_s);

    free(op_s);
    free(r1_s);
    free(r2_s);
    free_irr(&irr);
}

