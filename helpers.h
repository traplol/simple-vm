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

void print_dissassembly(unsigned int ins);

static inline int is_printable(char c) {
    return c > 0x1f && c < 0x7f;
}

static inline int get_opcode(unsigned int ins) {
    return (ins & (OPCODE_MASK << OPCODE_SHIFT)) >> OPCODE_SHIFT;
}
static inline int get_r1(unsigned int ins) {
    return (ins & (REGISTER_MASK << R1_SHIFT)) >> R1_SHIFT;
}
static inline int get_r2(unsigned int ins) {
    return (ins & (REGISTER_MASK << R2_SHIFT)) >> R2_SHIFT;
}
static inline int get_imm(unsigned int ins) {
    int sign_bit = ins & SIGN_BIT_MASK_21BIT;
    if (sign_bit) {
        sign_bit |= SIGN_BIT_MASK_21BIT_TO_32BIT;
    }
    return (ins & IMMEDIATE_MASK) | sign_bit;
}

#endif

