#include <stdlib.h> /* calloc */
#include <stdio.h> /* snprintf */
#include <string.h> /* strlen, memcpy */

#include "helpers.h"

char *op_to_str(opcode_t op) {
    switch (op) {
        case nop:  return "nop";
        case add:  return "add";
        case mul:  return "mul";
 //       case div:  return "div";
        case cmp:  return "cmp";
        case and:  return "and";
        case or:   return "or";
        case xor:  return "xor";
        case sll:  return "sll";
        case srl:  return "srl";

        case addi: return "addi";
        case muli: return "muli";
        case divi: return "divi";
        case li:   return "li";
        case la:   return "la";

        case jr:   return "jr";
        case j:    return "j";
        case jz:   return "jz";
    }
    return "'not an opcode'";
}

char *reg_to_str(register_t r) {
    switch (r) {
        case null: return "null";
        case g0:   return "g0";
        case g1:   return "g1";
        case g2:   return "g2";
        case g3:   return "g3";
        case g4:   return "g4";
        case g5:   return "g5";
        case g6:   return "g6";
        case g7:   return "g7";

        case f0:   return "f0";
        case f1:   return "f1";
        case f2:   return "f2";
        case f3:   return "f3";

        case l0:   return "l0";
        case l1:   return "l1";
        case l2:   return "l2";
        case l3:   return "l3";

        case r0:   return "r0";
        case r1:   return "r1";

        case z:    return "z";
        case sp:   return "sp";
        case fp:   return "fp";
        case pc:   return "pc";
    }
    return "'not a register'";
}

char *imm_to_str(signed int imm, char *fmt) {
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

/* Returbs a new string with all of the strings passes concatenated. */
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

