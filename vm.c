#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "irr.h"
#include "irm.h"
#include "helpers.h"

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

int main(void) {
    test_irm(addi, g5, -99);
    test_irm(addi, g5, 99);
    test_irr(add, g4, g7);

    return 0;
}

