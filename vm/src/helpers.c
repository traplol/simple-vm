#include <stdlib.h> /* calloc */
#include <stdio.h> /* snprintf */
#include <string.h> /* strlen, memcpy */

#include "helpers.h"
#include "instruction.h"

void assert(int cond, char *msg) {
    if (!cond) {
        puts(msg);
    }
}

/* Returns a new copy of str with [A-Z] as [a-z] */
char *tolower(char *str) {
    char *ret = calloc(strlen(str)+1, sizeof *ret);
    for (size_t i = 0; i < strlen(str); ++i) {
        char c = str[i];
        if (c >= 'A' && c <= 'Z') {
            c += 'a' - 'A';
        }
        ret[i] = c;
    }
    return ret;
}

int count_char(char *str, char c) {
    int i;
    for (i = 0; *str != 0; ++str) {
        if (*str == c) { ++i; }
    }
    return i;
}


char *strdup(char *str) {
    size_t len = 1 + strlen(str);
    char *s = malloc(len);
    return s ? memcpy(s, str, len) : NULL;
}

/* Returns of a zero terminated copy of str - str+num */
char *substr(char *str, int num) {
    char *s = malloc(num+1);
    s[num] = 0;
    return s ? memcpy(s, str, num) : NULL;
}

/* Returns whether an opcude uses a PC relative offset. */
int is_pc_relative(opcode_t op) {
    return op == JZS || op == JS;
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

int get_num_operands(opcode_t op) {
    switch (get_type(op)) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE: return -1;

        case NO_OPERANDS: return 0;

        case REGISTER_REGISTER:
        case REGISTER_IMMEDIATE: return 2;

        case REGISTER_NO_IMMEDIATE:
        case IMMEDIATE_NO_REGISTER: return 1;
    }
}

char *op_to_str(opcode_t op) {
    switch (op) {
        case OPCODE_COUNT:
        case NOT_AN_OPCODE:
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
        case NOT_A_REGISTER:
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

opcode_t str_to_op(char *str) {
    int op = -1;
    str = tolower(str);
    if (strcmp(str, "halt") == 0) { op = HALT; }
    else if (strcmp(str, "nop") == 0) { op = NOP; }

    else if (strcmp(str, "add") == 0) { op = ADD; }
    else if (strcmp(str, "mul") == 0) { op = MUL; }
    else if (strcmp(str, "div") == 0) { op = DIV; }
    else if (strcmp(str, "eq") == 0) { op = EQ; }
    else if (strcmp(str, "ne") == 0) { op = NE; }
    else if (strcmp(str, "lt") == 0) { op = LT; }
    else if (strcmp(str, "gt") == 0) { op = GT; }
    else if (strcmp(str, "ge") == 0) { op = GE; }
    else if (strcmp(str, "and") == 0) { op = AND; }
    else if (strcmp(str, "or") == 0) { op = OR; }
    else if (strcmp(str, "xor") == 0) { op = XOR; }
    else if (strcmp(str, "sll") == 0) { op = SLL; }
    else if (strcmp(str, "srl") == 0) { op = SRL; }
    else if (strcmp(str, "mov") == 0) { op = MOV; }
    else if (strcmp(str, "lw") == 0) { op = LW; }
    else if (strcmp(str, "sw") == 0) { op = SW; }

    else if (strcmp(str, "addi") == 0) { op = ADDI; }
    else if (strcmp(str, "muli") == 0) { op = MULI; }
    else if (strcmp(str, "divi") == 0) { op = DIVI; }
    else if (strcmp(str, "li") == 0) { op = LI; }

    else if (strcmp(str, "jr") == 0) { op = JR; }
    else if (strcmp(str, "push") == 0) { op = PUSH; }
    else if (strcmp(str, "pop") == 0) { op = POP; }

    else if (strcmp(str, "j") == 0) { op = J; }
    else if (strcmp(str, "js") == 0) { op = JS; }
    else if (strcmp(str, "jz") == 0) { op = JZ; }
    else if (strcmp(str, "jzs") == 0) { op = JZS; }
    else if (strcmp(str, "call") == 0) { op = CALL; }
    else if (strcmp(str, "pushi") == 0) { op = PUSHI; }
    free(str);
    return op;
}

register_t str_to_reg(char *str) {
    int reg = -1;
    str = tolower(str);
    if (strcmp(str, "nul") == 0) { reg = NUL; }
    else if (strcmp(str, "g0") == 0) { reg = G0; }
    else if (strcmp(str, "g1") == 0) { reg = G1; }
    else if (strcmp(str, "g2") == 0) { reg = G2; }
    else if (strcmp(str, "g3") == 0) { reg = G3; }
    else if (strcmp(str, "g4") == 0) { reg = G4; }
    else if (strcmp(str, "g5") == 0) { reg = G5; }
    else if (strcmp(str, "g6") == 0) { reg = G6; }
    else if (strcmp(str, "g7") == 0) { reg = G7; }
    else if (strcmp(str, "g8") == 0) { reg = G8; }
    else if (strcmp(str, "g9") == 0) { reg = G9; }
    else if (strcmp(str, "g10") == 0) { reg = G10; }
    else if (strcmp(str, "g11") == 0) { reg = G11; }

    else if (strcmp(str, "f0") == 0) { reg = F0; }
    else if (strcmp(str, "f1") == 0) { reg = F1; }
    else if (strcmp(str, "f2") == 0) { reg = F2; }
    else if (strcmp(str, "f3") == 0) { reg = F3; }

    else if (strcmp(str, "r0") == 0) { reg = R0; }
    else if (strcmp(str, "r1") == 0) { reg = R1; }

    else if (strcmp(str, "z") == 0) { reg = Z; }
    else if (strcmp(str, "sp") == 0) { reg = SP; }
    else if (strcmp(str, "fp") == 0) { reg = FP; }
    else if (strcmp(str, "pc") == 0) { reg = PC; }
    free(str);
    return reg;
}


