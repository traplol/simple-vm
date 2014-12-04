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
    HALT = 0,
    NOP,

    /* register to register */
    /* oooooo rrrrr rrrrr 0000000000000000 */
    /*    6     5     5         16         */
    ADD,
    MUL,
    DIV,
    CMP,
    AND,
    OR,
    XOR,
    SLL,
    SRL,
    MOV,

    /* immediate to register */
    /* oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm */
    /*    6     5           21            */
    ADDI,
    MULI,
    DIVI,
    LI,

    /* jmp register */
    /* oooooo rrrrr 000000000000000000000 */
    /*    6     5            21           */
    JR,

    /* jmp immediate */
    /* oooooo 00000 mmmmmmmmmmmmmmmmmmmmm  */
    /*    6                  21            */
    J,
    JS,
    JZ,
    JZS,
    CALL,

    /* The number of opcodes. */
    OPCODE_COUNT
} opcode_t;


typedef enum registers {
    NUL = 0,                                    /* 1 */
    /* General purpose 32-bit registers. */
    G0, G1, G2, G3, G4, G5, G6, G7,             /* 8 */
    /* 32-bit floating point registers. */
    F0, F1, F2, F3,                             /* 4 */
    /* 64-bit registers. */
    L0, L1, L2, L3,                             /* 4 */
    /* Return registers. */
    R0, R1,                                     /* 2 */
    /* Comparison flag. */
    Z,                                          /* 1 */
    /* Stack pointer. */
    SP,                                         /* 1 */
    /* Frame pointer. */
    FP,                                         /* 1 */
    /* Program counter. */
    PC,                                         /* 1 */

    /* The number of registers. */
    REGISTER_COUNT
} register_t;                                   /* 23 */

#endif

