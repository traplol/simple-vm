#include <stdlib.h> /* malloc, free */

#include "irm.h"
#include "helpers.h"

irm_t *make_irm(opcode_t op, register_t r, signed int imm) {
    irm_t *irm = malloc(sizeof *irm);
    irm->op = op;
    irm->r = r;
    irm->imm = imm;
    irm->op_str = op_to_str(op);
    irm->r_str = reg_to_str(r);
    irm->imm_str = imm_to_str(imm, "%d");
    irm->compiled_str = str_cat(5, irm->op_str, " ", irm->r_str, ", ", irm->imm_str);
    return irm;
}

void free_irm(irm_t **irm) {
    if (irm && *irm) {
        free((*irm)->imm_str);
        free((*irm)->compiled_str);
        free(*irm);
    }
}

irm_t *decompile_irm(unsigned int ins) {
    opcode_t op = ins & (OPCODE_MASK << OPCODE_SHIFT);
    op >>= OPCODE_SHIFT;

    register_t r = ins & (REGISTER_MASK << R1_SHIFT);
    r >>= R1_SHIFT;

    /* Get sign bit from instruction. */
    unsigned int sign_bit = ins & SIGN_BIT_MASK_21BIT;
    if (sign_bit) {
        /* Flip bits 22-32 to 1 if sign bit is flipped. */
        sign_bit |= SIGN_BIT_MASK_21BIT_TO_32BIT;
    }
    /* Get the immediate value from the instruction. */
    signed int imm = ins & (IMMEDIATE_MASK);
    /* Restore the sign bit */
    imm |= sign_bit;

    return make_irm(op, r, imm);
}

/* immediate to register */
/* oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm */
/*    6     5           21            */
unsigned int compile_irm(opcode_t op, register_t r, signed int imm) {
    /* Get the sign bit. */
    unsigned int sign_bit = (imm & SIGN_BIT_MASK_32BIT) >> SIGN_BIT_SHIFT_21BIT;
    /* chop off bits 22-32 */
    imm &= IMMEDIATE_MASK; 
    /* Restore sign bit. */
    imm |= sign_bit;
    /* register */
    imm |= (r << R1_SHIFT);
    /* opcode */
    return imm | (op << OPCODE_SHIFT);
}

/* Tests that a compiled irm format decompiles back to the arguments passes. */
void test_irm(opcode_t test_op, register_t test_reg, signed int test_imm) {
    unsigned int test = compile_irm(test_op, test_reg, test_imm);
    irm_t *irm = decompile_irm(test);

    char *op_s, *r_s, *imm_s, *test_imm_s;
    test_imm_s = imm_to_str(test_imm, "%d");

    op_s = str_cat(4, "irm: ", irm->op_str, " != ", op_to_str(test_op));
    r_s = str_cat(4, "irm: ", irm->r_str, " != ", reg_to_str(test_reg));
    imm_s = str_cat(4, "irm: ", irm->imm_str, " != ", test_imm_s);

    assert(irm->op == test_op, op_s);
    assert(irm->r == test_reg, r_s);
    assert(irm->imm == test_imm, imm_s);

    free(op_s);
    free(r_s);
    free(imm_s);
    free(test_imm_s);
    free_irm(&irm);
}

