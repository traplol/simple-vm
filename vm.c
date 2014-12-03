#include <stdio.h>


#define OPCODE_MASK (0x3f)
#define REGISTER_MASK (0x1f)
#define IMMEDIATE_MASK (0x1fffff)
#define SIGN_MASK_32BIT (0x80000000)

#define R1_SHIFT (5 + 16)
#define R2_SHIFT (16)
#define OPCODE_SHIFT (5 + 5 + 16)
#define SIGN_BIT_SHIFT (11)

void assert(int cond, char *msg) {
    if (!cond) {
        puts(msg);
    }
}

typedef enum opcode {
    /* register to register */
    /* oooooo rrrrr rrrrr 0000000000000000*/
    /*    6     5     5         16        */
    nop = 0,
    add,
    div,
    cmp,
    la,
    and,
    or,
    xor,

    /* immediate to register */
    /* oooooo rrrrr mmmmmmmmmmmmmmmmmmm */
    /*    6     5           21          */
    addi,
    divi,
    li,

    /* jmp */
    /* oooooo rrrrr 00000000000000000000000000 */

} opcode_t;


typedef enum registers {
    null = 0,
    /* General purpose 32-bit registers. */
    g0, g1, g2, g3, g4, g5, g6, g7,
    /* 32-bit floating point registers. */
    f0, f1, f2, f3,
    /* 64-bit registers. */
    l0, l1, l2, l3,
    /* Stack pointer. */
    sp,
    /* Frame pointer. */
    fp,
    /* Return registers. */
    r1, r2
} register_t;

typedef struct irr {
    opcode_t op;
    register_t r1, r2;
} irr_t;

typedef struct irm {
    opcode_t op;
    register_t r;
    int imm;
} irm_t;

irm_t decompile_irm(unsigned int ins) {
    irm_t irm;
    irm.op = ins & (OPCODE_MASK << OPCODE_SHIFT);
    irm.op >>= OPCODE_SHIFT;

    irm.r = ins & (REGISTER_MASK << R1_SHIFT);
    irm.r >>= R1_SHIFT;

    irm.imm = ins & (IMMEDIATE_MASK);
    return irm;
}

irr_t decompile_irr(unsigned int ins) {
    irr_t irr;
    irr.op = ins & (OPCODE_MASK << OPCODE_SHIFT);
    irr.op >>= OPCODE_SHIFT;

    irr.r1 = ins & (REGISTER_MASK << R1_SHIFT);
    irr.r1 >>= R1_SHIFT;

    irr.r2 = ins & (REGISTER_MASK << R2_SHIFT);
    irr.r2 >>= R2_SHIFT;
    return irr;
}


/* register to register */
/* oooooo rrrrr rrrrr 0000000000000000*/
/*    6     5     5         16        */
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

/* immediate to register */
/* oooooo rrrrr mmmmmmmmmmmmmmmmmmm */
/*    6     5           21          */
unsigned int compile_irm(opcode_t op, register_t r, signed int imm) {
    /* Get the sign bit. */
    unsigned int sign_bit = (imm & SIGN_MASK_32BIT) >> SIGN_BIT_SHIFT;
    /* chop off bits 22-32 */
    imm &= IMMEDIATE_MASK; 
    /* Restore sign bit. */
    imm |= sign_bit;
    /* register */
    imm |= (r << R1_SHIFT);
    /* opcode */
    return imm | (op << OPCODE_SHIFT);
}

void test_irm(opcode_t test_op, register_t test_reg, signed int test_imm) {
    unsigned int test = compile_irm(test_op, test_reg, test_imm);
    irm_t irm = decompile_irm(test); 
    assert(irm.op == test_op, "irm: op != test_op");
    assert(irm.r == test_reg, "irm: r != test_reg");
    assert(irm.imm == test_imm, "irm: imm != test_imm");
}
void test_irr(opcode_t test_op, register_t test_r1, register_t test_r2) {
    unsigned int test = compile_irr(test_op, test_r1, test_r2);
    irr_t irr = decompile_irr(test);
    assert(irr.op == test_op, "irr: op != test_op");
    assert(irr.r1 == test_r1, "irr: r1 != test_r1");
    assert(irr.r2 == test_r2, "irr: r2 != test_r2");
}

void execute() {
}

int main(void) {
    test_irm(addi, g5, 99);
    test_irr(add, g4, g7);

    return 0;
}

