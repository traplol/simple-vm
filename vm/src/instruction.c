#include <stdlib.h> /* NULL, malloc, free */
#include <stdio.h> /* printf */
#include "instruction.h"
#include "helpers.h"

void do_strings(instruction_t *ins) {
    ins->opcode_str = strdup(op_to_str(ins->opcode));
    ins->operand1_str = NULL;
    ins->operand2_str = NULL;
    ins->operand3_str = NULL;
    switch(ins->type) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE:
            return;
        case NO_OPERANDS:
            ins->disassembled_str = str_cat(1, ins->opcode_str);
            return;
        case REGISTER_REGISTER:
            ins->operand1_str = strdup(reg_to_str(ins->operand1.reg));
            ins->operand2_str = strdup(reg_to_str(ins->operand2.reg));
            ins->disassembled_str =
                str_cat(5, ins->opcode_str, " ", ins->operand1_str, ", ", ins->operand2_str);
            return;
        case REGISTER_REGISTER_OFFSET:
            ins->operand1_str = strdup(reg_to_str(ins->operand1.reg));
            ins->operand2_str = strdup(reg_to_str(ins->operand2.reg));
            ins->operand3_str = imm_to_str(ins->operand3.imm, "%d");
            ins->disassembled_str =
                str_cat(7, ins->opcode_str, " ", ins->operand1_str, ", ", ins->operand2_str, ", ", ins->operand3_str);
            return;
        case REGISTER_IMMEDIATE:
            ins->operand1_str = strdup(reg_to_str(ins->operand1.reg));
            ins->operand2_str = imm_to_str(ins->operand2.imm, "%d");
            ins->disassembled_str =
                str_cat(5, ins->opcode_str, " ", ins->operand1_str, ", ", ins->operand2_str);
            return;
        case REGISTER_NO_IMMEDIATE:
            ins->operand1_str = strdup(reg_to_str(ins->operand1.reg));
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

void do_operands(instruction_t *ins) {
    switch(ins->type) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE:
            return;
        case NO_OPERANDS:
            ins->operand1.imm = 0;
            ins->operand2.imm = 0;
            return;
        case REGISTER_REGISTER:
            ins->operand1.reg = get_r1(ins->assembled_value);
            ins->operand2.reg = get_r2(ins->assembled_value);
            return;
        case REGISTER_REGISTER_OFFSET:
            ins->operand1.reg = get_r1(ins->assembled_value);
            ins->operand2.reg = get_r2(ins->assembled_value);
            ins->operand3.imm = get_imm16(ins->assembled_value);
            return;
        case REGISTER_IMMEDIATE:
            ins->operand1.reg = get_r1(ins->assembled_value);
            ins->operand2.imm = get_imm21(ins->assembled_value);
            return;
        case REGISTER_NO_IMMEDIATE:
            ins->operand1.reg = get_r1(ins->assembled_value);
            ins->operand2.imm = 0;
            return;
        case IMMEDIATE_NO_REGISTER:
            ins->operand1.imm = get_imm21(ins->assembled_value);
            ins->operand2.imm = 0;
            return;
    }
}

instruction_t *make_no_operand_instruction(opcode_t opcode) {
    return make_instruction(opcode, 0, 0, 0);
}
instruction_t *make_one_operand_instruction(opcode_t opcode, int32_t operand1) {
    return make_instruction(opcode, operand1, 0, 0);
}
instruction_t *make_two_operand_instruction(opcode_t opcode, int32_t operand1, int32_t operand2) {
    return make_instruction(opcode, operand1, operand2, 0);
}
instruction_t *make_three_operand_instruction(opcode_t opcode, int32_t operand1, int32_t operand2, int32_t operand3) {
    return make_instruction(opcode, operand1, operand2, operand3);
}

/* Makes a new instruction with all of the components to print,
 * and the assembled value of the instruction. */
instruction_t *make_instruction(opcode_t opcode, int32_t operand1, int32_t operand2, int32_t operand3) {
    instruction_t *ins = malloc(sizeof *ins);
    ins->type = get_type(opcode);
    ins->opcode = opcode;
    ins->assembled_value = assemble_instruction(opcode, operand1, operand2, operand3);
    do_operands(ins);
    do_strings(ins);
    return ins;
}

/* Frees the instruction and sets it equal to null. */
void free_instruction(instruction_t **instruction) {
    instruction_t *ins;
    if (instruction && *instruction) {
        ins = *instruction;
        free(ins->opcode_str);
        free(ins->operand1_str);
        free(ins->operand2_str);
        free(ins->operand3_str);
        free(ins->disassembled_str);
        free(ins);
        *instruction = NULL;
    }
}

int32_t assemble_no_operands(opcode_t opcode) {
    return opcode << OPCODE_SHIFT;
}
int32_t assemble_register_register(opcode_t op, register_t r1, register_t r2 ) {
    int32_t ins = 0;
    ins |= (op << OPCODE_SHIFT);
    ins |= (r1 << R1_SHIFT);
    ins |= (r2 << R2_SHIFT);
    return ins;
}
int32_t assemble_register_immediate(opcode_t op, register_t r, int32_t imm) {
    int32_t ins = 0;
    int32_t sign_bit = (imm & SIGN_BIT_MASK_32BIT) >> SIGN_BIT_SHIFT_21BIT;
    imm &= IMMEDIATE_MASK_21BIT;
    imm |= sign_bit;

    ins |= (op << OPCODE_SHIFT);
    ins |= (r << R1_SHIFT);
    ins |= imm;
    return ins;
}
int32_t assemble_register_no_immediate(opcode_t op, register_t r) {
    int32_t ins = 0;
    ins |= (op << OPCODE_SHIFT);
    ins |= (r << R1_SHIFT);
    return ins;
}
int32_t assemble_register_register_offset(opcode_t op, register_t r1, register_t r2,int32_t imm) {
    int32_t ins = assemble_register_register(op, r1, r2);

    int32_t sign_bit = (imm & SIGN_BIT_MASK_32BIT) >> SIGN_BIT_SHIFT_16BIT;
    imm &= IMMEDIATE_MASK_16BIT;
    imm |= sign_bit;

    ins |= imm;
    return ins;
}
int32_t assemble_immediate_no_register(opcode_t op, int32_t imm) {
    int32_t ins = 0;
    int32_t sign_bit = (imm & SIGN_BIT_MASK_32BIT) >> SIGN_BIT_SHIFT_21BIT;
    imm &= IMMEDIATE_MASK_21BIT;
    imm |= sign_bit;


    ins |= (op << OPCODE_SHIFT);
    ins |= imm;
    return ins;
}

/* Compiles an instruction to its binary representation. */
int32_t assemble_instruction(opcode_t opcode, int32_t operand1, int32_t operand2, int32_t operand3) {
    instruction_type_t ty = get_type(opcode);

    switch(ty) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE:
            return -1;
        case NO_OPERANDS:
            return assemble_no_operands(opcode);
        case REGISTER_REGISTER:
            return assemble_register_register(opcode, operand1, operand2);
        case REGISTER_REGISTER_OFFSET:
            return assemble_register_register_offset(opcode, operand1, operand2, operand3);
        case REGISTER_IMMEDIATE:
            return assemble_register_immediate(opcode, operand1, operand2);
        case REGISTER_NO_IMMEDIATE:
            return assemble_register_no_immediate(opcode, operand1);
        case IMMEDIATE_NO_REGISTER:
            return assemble_immediate_no_register(opcode, operand1);
    }
}


instruction_t *disassemble_instruction(int32_t instruction) {
    opcode_t op = get_opcode(instruction);
    register_t r1 = get_r1(instruction);
    register_t r2 = get_r2(instruction);
    int imm21 = get_imm21(instruction);
    int imm16 = get_imm16(instruction);
    instruction_type_t ty = get_type(op);
    switch(ty) {
        case INSTRUCTION_TYPE_COUNT:
        case INVALID_INSTRUCTION_TYPE:
            return NULL;
        case NO_OPERANDS:
            return make_no_operand_instruction(op);
        case REGISTER_REGISTER:
            return make_two_operand_instruction(op, r1, r2);
        case REGISTER_REGISTER_OFFSET:
            return make_three_operand_instruction(op, r1, r2, imm16);
        case REGISTER_IMMEDIATE:
            return make_two_operand_instruction(op, r1, imm21);
        case REGISTER_NO_IMMEDIATE:
            return make_one_operand_instruction(op, r1);
        case IMMEDIATE_NO_REGISTER:
            return make_one_operand_instruction(op, imm21);
    }
}


instruction_type_t get_type(opcode_t opcode) {
    switch(opcode) {
        case OPCODE_COUNT:
        case NOT_AN_OPCODE:
            return INVALID_INSTRUCTION_TYPE;

        case HALT:
        case NOP:
        case RET:
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
            return REGISTER_REGISTER;

        case LW:
        case SW:
            return REGISTER_REGISTER_OFFSET;

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


