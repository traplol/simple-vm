#ifndef SIMPLE_VM_HELPERS_H
#define SIMPLE_VM_HELPERS_H
#include "opdefs.h"
#include "typedefs.h"

/* Returns the opcode as a readable string. */
char *op_to_str(opcode_t op);
/* Returns the register as a readable string. */
char *reg_to_str(register_t r);
/* Returns the immediate as a readable string.
 * Note: This needs to be freed. */
char *imm_to_str(i32 imm, char *fmt);

/* Takes a string and returns the appropriate opcode. */
opcode_t str_to_op(char *str);
/* Takes a string and returns the appropriate register. */
register_t str_to_reg(char *str);

/* Returns the number of operands for an opcode, or -1 if it's not an opcode */
i32 get_num_operands(opcode_t op);

/* Returns whether an opcude uses a PC relative offset. */
i32 is_pc_relative(opcode_t op);

/* Returns a new string with all of the strings passes concatenated. 
 * Note: This needs to be freed. */
char *str_cat(size_t count, ...);

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
void print_dissassembly(int32_t ins);

/* Returns if a character is visibly printable. */
static inline i32 is_printable(i8 c) {
    return c > 0x1f && c < 0x7f;
}
static inline i32 get_opcode(i32 ins) {
    return (ins >> OPCODE_SHIFT) & OPCODE_MASK;
}
static inline i32 get_r1(i32 ins) {
    return (ins >> R1_SHIFT) & REGISTER_MASK;
}
static inline i32 get_r2(i32 ins) {
    return (ins >> R2_SHIFT) & REGISTER_MASK;
}
static inline i32 get_imm21(i32 ins) {
    i32 sign_bit = ins & SIGN_BIT_MASK_21BIT;
    if (sign_bit) {
        sign_bit |= SIGN_BIT_MASK_21BIT_TO_32BIT;
    }
    return (ins & IMMEDIATE_MASK_21BIT) | sign_bit;
}
static inline i32 get_imm16(i32 ins) {
    i32 sign_bit = ins & SIGN_BIT_MASK_16BIT;
    if (sign_bit) {
        sign_bit |= SIGN_BIT_MASK_16BIT_TO_32BIT;
    }
    return (ins & IMMEDIATE_MASK_16BIT) | sign_bit;
}

#endif

