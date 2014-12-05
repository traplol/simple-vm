#include <stdio.h>
#include "smart-compile.h"
#include "irm.h"
#include "irr.h"

unsigned int smart_compile(opcode_t opcode, int operand1, int operand2) {
    switch (opcode) {
        case OPCODE_COUNT:
            fprintf(stderr, "%d - (0x%x) is not a valid opcode.\n", opcode, opcode);

        case HALT:
            return HALT;
        case NOP:
            return NOP;

    /* register to register */
    /* oooooo rrrrr rrrrr 0000000000000000 */
    /*    6     5     5         16         */
        case ADD:
        case MUL:
        case DIV:
        case CMP:
        case AND:
        case OR:
        case XOR:
        case SLL:
        case SRL:
        case MOV:
        case LW:
        case SW:
            return compile_irr(opcode, operand1, operand2);
    /* immediate to register */
    /* oooooo rrrrr mmmmmmmmmmmmmmmmmmmmm */
    /*    6     5           21            */
        case ADDI:
        case MULI:
        case DIVI:
        case LI:
            return compile_irm(opcode, operand1, operand2);
    /* jmp register */
    /* oooooo rrrrr 000000000000000000000 */
        case JR:
        case PUSH:
        case POP:
            return compile_irm(opcode, operand1, NUL);
    /* jmp immediate */
    /* oooooo 00000 mmmmmmmmmmmmmmmmmmmmm  */
    /*    6                  21            */
        case J:
        case JS:
        case JZ:
        case JZS:
        case CALL:
        case PUSHI:
            return compile_irm(opcode, NUL, operand2);
    }

}

