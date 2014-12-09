#ifndef SIMPLE_VM_HELPERS_H
#define SIMPLE_VM_HELPERS_H
#include "opdefs.h"

/* Returns the opcode as a readable string. */
char *op_to_str(opcode_t op);
/* Returns the register as a readable string. */
char *reg_to_str(register_t r);
/* Returns the immediate as a readable string.
 * Note: This needs to be freed. */
char *imm_to_str(int imm, char *fmt);

/* Takes a string and returns the appropriate opcode. */
opcode_t str_to_op(char *str);
/* Takes a string and returns the appropriate register. */
register_t str_to_reg(char *str);

/* Returns the number of operands for an opcode, or -1 if it's not an opcode */
int get_num_operands(opcode_t op);

/* Returns whether an opcude uses a PC relative offset. */
int is_pc_relative(opcode_t op);

/* Returns a new string with all of the strings passes concatenated. 
 * Note: This needs to be freed. */
char *str_cat(unsigned long count, ...);

/* Prints a message to stderr if condition is false. */
void assert(int cond, char *msg);

/* Returns a copy of str. */
char *strdup(char *str);

/* Returns of a zero terminated copy of str - str+num */
char *substr(char *str, int num);

/* Returns a lowercase copy of str. */
char *to_lower(char *str);

/* Counts the occurences of c in str. */
int count_char(char *str, char c);

/* Prints an instruction in disassembled format. */
void print_dissassembly(unsigned int ins);

/* Returns if a character is visibly printable. */
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

