#include <stdlib.h> /* NULL, malloc, free */
#include "instruction.h"
#include "helpers.h"

void make_strings(instruction_t *ins) {
    ins->opcode_str = op_to_str(ins->opcode);
    switch(ins->type) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE:
            return;
        case NO_OPERANDS:
            ins->disassembled_str = str_cat(1, ins->opcode_str);
            return;
        case REGISTER_REGISTER:
            ins->operand1_str = reg_to_str(ins->operand1.reg);
            ins->operand2_str = reg_to_str(ins->operand2.reg);
            ins->disassembled_str =
                str_cat(5, ins->opcode_str, " ", ins->operand1_str, ", ", ins->operand2_str);
            return;
        case REGISTER_IMMEDIATE:
            ins->operand1_str = reg_to_str(ins->operand1.reg);
            ins->operand2_str = imm_to_str(ins->operand2.imm, "%d");
            ins->disassembled_str =
                str_cat(5, ins->opcode_str, " ", ins->operand1_str, ", ", ins->operand2_str);
            return;
        case REGISTER_NO_IMMEDIATE:
            ins->operand1_str = reg_to_str(ins->operand1.reg);
            ins->disassembled_str =
                str_cat(3, ins->opcode_str, " ", ins->operand1_str);
            return;
        case IMMEDIATE_NO_REGISTER:
            ins->operand1_str = imm_to_str(ins->operand1.imm, "%d");
            ins->disassembled_str =
                str_cat(3, ins->opcode_str, " ", ins->operand1_str);
            return;
    }
}

instruction_t *make_no_operand_instruction(opcode_t opcode) {
    return make_instruction(opcode, 0, 0);
}
instruction_t *make_one_operand_instruction(opcode_t opcode, int operand1) {
    return make_instruction(opcode, operand1, 0);
}
instruction_t *make_two_operand_instruction(opcode_t opcode, int operand1, int operand2) {
    return make_instruction(opcode, operand1, operand2);
}

/* Makes a new instruction with all of the components to print,
 * and the compiled value of the instruction. */
instruction_t *make_instruction(opcode_t opcode, int operand1, int operand2) {
    instruction_t *ins = malloc(sizeof *ins);
    ins->type = get_type(opcode);
    ins->opcode = opcode;
    ins->operand1.reg = operand1;
    ins->operand1.imm = operand1;
    ins->operand2.reg = operand2;
    ins->operand2.imm = operand2;
    ins->compiled_value = compile_instruction(opcode, operand1, operand2);
    make_strings(ins);
    return ins;
}

/* Frees the instruction and sets it equal to null. */
void free_instruction(instruction_t **instruction) {
    instruction_t *ins;
    if (instruction && *instruction) {
        ins = *instruction;
        if (ins->type == IMMEDIATE_NO_REGISTER) {
            free(ins->operand1_str);
        }
        if (ins->type == REGISTER_IMMEDIATE) {
            free(ins->operand2_str);
        }
        free(ins->disassembled_str);
        free(ins);
        *instruction = NULL;
    }
}

unsigned int compile_no_operands(opcode_t opcode) {
    return opcode << OPCODE_SHIFT;
}
unsigned int compile_register_register(opcode_t op, register_t r1, register_t r2 ) {
    unsigned int ins = 0;
    ins |= (op << OPCODE_SHIFT);
    ins |= (r1 << R1_SHIFT);
    ins |= (r2 << R2_SHIFT);
    return ins;
}
unsigned int compile_register_immediate(opcode_t op, register_t r, int imm) {
    unsigned int ins = 0;
    int sign_bit = (imm & SIGN_BIT_MASK_32BIT) >> SIGN_BIT_SHIFT_21BIT;
    imm &= IMMEDIATE_MASK;
    imm |= sign_bit;

    ins |= (op << OPCODE_SHIFT);
    ins |= (r << R1_SHIFT);
    ins |= imm;
    return ins;
}
unsigned int compile_register_no_immediate(opcode_t op, register_t r) {
    unsigned int ins = 0;
    ins |= (op << OPCODE_SHIFT);
    ins |= (r << R1_SHIFT);
    return ins;
}
unsigned int compile_immediate_no_register(opcode_t op, int imm) {
    unsigned int ins = 0;
    int sign_bit = (imm & SIGN_BIT_MASK_32BIT) >> SIGN_BIT_SHIFT_21BIT;
    imm &= IMMEDIATE_MASK;
    imm |= sign_bit;


    ins |= (op << OPCODE_SHIFT);
    ins |= imm;
    return ins;
}

/* Compiles an instruction to it's binary representation. */
unsigned int compile_instruction(opcode_t opcode, int operand1, int operand2) {
    instruction_type_t ty = get_type(opcode);

    switch(ty) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE:
            return -1;
        case NO_OPERANDS:
            return compile_no_operands(opcode);
        case REGISTER_REGISTER:
            return compile_register_register(opcode, operand1, operand2);
        case REGISTER_IMMEDIATE:
            return compile_register_immediate(opcode, operand1, operand2);
        case REGISTER_NO_IMMEDIATE:
            return compile_register_no_immediate(opcode, operand1);
        case IMMEDIATE_NO_REGISTER:
            return compile_immediate_no_register(opcode, operand1);
    }
}


instruction_type_t get_type(opcode_t opcode) {
    switch(opcode) {
        case OPCODE_COUNT:
            return INVALID_INSTRUCTION_TYPE;

        case HALT:
        case NOP:
            return NO_OPERANDS;

        case ADD:
        case MUL:
        case DIV:
        case EQ:
        case NE:
        case LT:
        case LE:
        case GT:
        case GE:
        case AND:
        case OR:
        case XOR:
        case SLL:
        case SRL:
        case MOV:
        case LW:
        case SW:
            return REGISTER_REGISTER;

        case ADDI:
        case MULI:
        case DIVI:
        case LI:
            return REGISTER_IMMEDIATE;

        case JR:
        case PUSH:
        case POP:
            return REGISTER_NO_IMMEDIATE;

        case J:
        case JS:
        case JZ:
        case JZS:
        case CALL:
        case PUSHI:
            return IMMEDIATE_NO_REGISTER;
    }

    return INVALID_INSTRUCTION_TYPE;
}


