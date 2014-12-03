#ifndef SIMPLE_VM_DEFS_H
#define SIMPLE_VM_DEFS_H

#define OPCODE_MASK (0x3f)
#define REGISTER_MASK (0x1f)
#define IMMEDIATE_MASK (0x1fffff)
#define SIGN_BIT_MASK_32BIT (0x80000000)
#define SIGN_BIT_MASK_26BIT (0x4000000)  /* NOT USED */
#define SIGN_BIT_MASK_21BIT (0x100000)
#define SIGN_BIT_MASK_21BIT_TO_32BIT (0xfff00000)

#define R1_SHIFT (5 + 16)
#define R2_SHIFT (16)
#define OPCODE_SHIFT (5 + 5 + 16)
#define SIGN_BIT_SHIFT_21BIT (11)

typedef enum opcode {
    /* register to register */
    /* oooooo rrrrr rrrrr 0000000000000000 */
    /*    6     5     5         16         */
    nop = 0,
    add,
    mul,
//    div,
    cmp,
    and,
    or,
    xor,
    sll,
    srl,

    /* immediate to register */
    /* oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm */
    /*    6     5           21            */
    addi,
    muli,
    divi,
    li,
    la,

    /* jmp register */
    /* oooooo rrrrr 000000000000000000000 */
    /*    6     5            21           */
    jr,
    /* jmp immediate */
    /* oooooo 00000 mmmmmmmmmmmmmmmmmmmmm  */
    /*    6                  21            */
    j,
    jz

} opcode_t;


typedef enum registers {
    null = 0,                                   /* 1 */
    /* General purpose 32-bit registers. */
    g0, g1, g2, g3, g4, g5, g6, g7,             /* 8 */
    /* 32-bit floating point registers. */
    f0, f1, f2, f3,                             /* 4 */
    /* 64-bit registers. */
    l0, l1, l2, l3,                             /* 4 */
    /* Return registers. */
    r0, r1,                                     /* 2 */
    /* Comparison flag. */
    z,                                          /* 1 */
    /* Stack pointer. */
    sp,                                         /* 1 */
    /* Frame pointer. */
    fp,                                         /* 1 */
    /* Program counter. */
    pc,                                         /* 1 */
} register_t;                                   /* 23 */

#endif

