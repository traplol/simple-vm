#include <stdlib.h> /* calloc */
#include <stdio.h> /* snprintf */
#include <string.h> /* strlen, memcpy */

#include "helpers.h"
#include "instruction.h"

char *op_to_str(opcode_t op) {
    switch (op) {
        case OPCODE_COUNT:
            return "'not an opcode'";

        case HALT: return "halt";
        case NOP:  return "nop";

        case ADD:  return "add";
        case MUL:  return "mul";
        case DIV:  return "div";
        case EQ:   return "eq";
        case NE:   return "ne";
        case LT:   return "lt";
        case LE:   return "le";
        case GT:   return "gt";
        case GE:   return "ge";
        case AND:  return "and";
        case OR:   return "or";
        case XOR:  return "xor";
        case SLL:  return "sll";
        case SRL:  return "srl";
        case MOV:  return "mov";
        case LW:   return "lw";
        case SW:   return "sw";

        case ADDI: return "addi";
        case MULI: return "muli";
        case DIVI: return "divi";
        case LI:   return "li";

        case JR:   return "jr";
        case PUSH: return "push";
        case POP:  return "pop";

        case J:    return "j";
        case JS:   return "js";
        case JZ:   return "jz";
        case JZS:  return "jzs";
        case CALL: return "call";
        case PUSHI:return "pushi";
    }
}

char *reg_to_str(register_t r) {
    switch (r) {
        case REGISTER_COUNT:
            return "'not a register'";

        case NUL:  return "nul";
        case G0:   return "g0";
        case G1:   return "g1";
        case G2:   return "g2";
        case G3:   return "g3";
        case G4:   return "g4";
        case G5:   return "g5";
        case G6:   return "g6";
        case G7:   return "g7";
        case G8:   return "g8";
        case G9:   return "g9";
        case G10:   return "g10";
        case G11:   return "g11";

        case F0:   return "f0";
        case F1:   return "f1";
        case F2:   return "f2";
        case F3:   return "f3";

        case R0:   return "r0";
        case R1:   return "r1";

        case Z:    return "z";
        case SP:   return "sp";
        case FP:   return "fp";
        case PC:   return "pc";
    }
}

char *imm_to_str(int imm, char *fmt) {
    const unsigned long len = 40;
    char *buf = calloc(len, sizeof *buf);
    snprintf(buf, len, fmt, imm);
    return buf;
}


void assert(int cond, char *msg) {
    if (!cond) {
        puts(msg);
    }
}

/* Returns a new string with all of the strings passes concatenated. */
char *str_cat(unsigned long count, ...) {
    size_t lengths[count];
    size_t total_length = 0;
    char *strings[count];
    va_list ap;
    va_start(ap, count);
    for (size_t i = 0; i < count; ++i) {
        strings[i] = va_arg(ap, char*);
        lengths[i] = strlen(strings[i]);
        total_length += lengths[i];
    }
    va_end(ap);

    char *ret = calloc(1 + total_length, sizeof *ret);
    char *ret_pos = ret;
    for (size_t i = 0; i < count; ++i) {
        memcpy(ret_pos, strings[i], lengths[i]);
        ret_pos += lengths[i];
    }
    return ret;
}

void print_dissassembly(unsigned int ins) {
    instruction_t *instruction = disassemble_instruction(ins);
    puts(instruction->disassembled_str);
    free_instruction(&instruction);
}
