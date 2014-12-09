#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "vm.h"
#include "helpers.h"
#include "instruction.h"


register_t registers[REGISTER_COUNT];
unsigned char memspace[MEMSIZE];
int32_t program_halted = 0;


void dump_registers(void) {
    char *str, *imm_str;
    for (int32_t i = 0; i < REGISTER_COUNT; ++i) {
        if (i >= F0 && i <= F3) {
            printf("%s: %f\n", reg_to_str(i), *((float*)registers + i));
        }
        else {
            imm_str = imm_to_str(registers[i], "%d");
            str = str_cat(3, reg_to_str(i), ": ", imm_str);
            puts(str);
            free(str);
            free(imm_str);
        }
    }
}

void dump_text_section(void) {
    int32_t col = 0;
    int32_t *start, *end;
    start = (int32_t*)(memspace+TEXT_SECTION_START);
    end = (int32_t*)(memspace+TEXT_SECTION_START + TEXT_SECTION_SIZE);
    for (col = 0; start != end; ++start, ++col) {
        if (col == 11) {
            printf("\n");
            col = 0;
        }
        printf(" %08x ", *start);
    }
    printf("\n");
}

void dump_data_section(void) {
    int32_t col = 0;
    unsigned char *tmp = &memspace[DATA_SECTION_START];
    unsigned char *end = &memspace[DATA_SECTION_START + DATA_SECTION_SIZE];
    unsigned char data, *start = tmp;
    for (; start != end; ++start, ++col) {
        data = *start;
        if (col == 19) {
            printf("\n");
            col = 0;
        }
        if (is_printable(data)) {
            printf("%2c ", data);
        }
        else {
            printf("%02x ", data);
        }
    }
    printf("\n");
}

void disassemble_program(unsigned char *program, size_t size) {
    for (size_t i = 0; i < size / sizeof (int32_t); ++i) {
        print_dissassembly(((int32_t*)program)[i]);
    }
}

void load_program(unsigned char *program, size_t size) {
    if (size > TEXT_SECTION_SIZE) {
        fputs("Program size too big.\n", stderr);
        exit(1);
    }
    memcpy(memspace+TEXT_SECTION_START, program, size);
}

void load_data(unsigned char *data, size_t size) {
    if (size > DATA_SECTION_SIZE) {
        fputs("Data size too big.\n", stderr);
        exit(1);
    }
    memcpy(memspace+DATA_SECTION_START, data, size);
}

void run(void) {
    size_t pc, ins;
    printf("%d", *(memspace+registers[PC]));
    while (program_halted == 0) {
        pc = registers[PC];
        ins = *((size_t*)(memspace+pc));
        execute(ins);
    }
}

void init(void) {
    memset(memspace, 0, MEMSIZE);
    memset(registers, 0, REGISTER_COUNT * sizeof (register_t));
    registers[PC] = TEXT_SECTION_START;
    registers[SP] = STACK_START;
    registers[FP] = STACK_START;
}

void execute(int32_t ins) {
    opcode_t op;
    register_t r1, r2;
    int32_t imm;

    op = get_opcode(ins);
    r1 = get_r1(ins);
    r2 = get_r2(ins);
    imm = get_imm(ins);


    switch (op) {
        case OPCODE_COUNT:
        case NOT_AN_OPCODE:
            fputs("'NOT AN OPCODE'", stderr);
            abort();
        case HALT:
            program_halted = 1;
            break;
        case NOP:
            registers[PC] += 4;
            break;
        case RET:
            registers[PC] = ((int32_t*)memspace)[registers[SP]];
            break;

        case ADD:
            registers[r1] += registers[r2];
            registers[PC] += 4;
            break;
        case MUL:
            registers[r1] *= registers[r2];
            registers[PC] += 4;
            break;
        case DIV:
            registers[r1] /= registers[r2];
            registers[PC] += 4;
            break;
        case EQ:
            registers[Z] = registers[r1] == registers[r2];
            registers[PC] += 4;
            break;
        case NE:
            registers[Z] = registers[r1] != registers[r2];
            registers[PC] += 4;
            break;
        case LT:
            registers[Z] = registers[r1] < registers[r2];
            registers[PC] += 4;
            break;
        case LE:
            registers[Z] = registers[r1] <= registers[r2];
            registers[PC] += 4;
            break;
        case GT:
            registers[Z] = registers[r1] > registers[r2];
            registers[PC] += 4;
            break;
        case GE:
            registers[Z] = registers[r1] >= registers[r2];
            registers[PC] += 4;
            break;
        case AND:
            registers[r1] &= registers[r2];
            registers[PC] += 4;
            break;
        case OR:
            registers[r1] |= registers[r2];
            registers[PC] += 4;
            break;
        case XOR:
            registers[r1] ^= registers[r2];
            registers[PC] += 4;
            break;
        case SLL:
            registers[r1] <<= registers[r2];
            registers[PC] += 4;
            break;
        case SRL:
            registers[r1] >>= registers[r2];
            registers[PC] += 4;
            break;
        case MOV:
            registers[r1] = registers[r2];
            registers[PC] += 4;
            break;
        case LW:
            registers[r1] = ((int32_t*)memspace)[registers[r2]];
            registers[PC] += 4;
            break;
        case SW:
            ((int32_t*)memspace)[registers[r2]] = registers[r1];
            registers[PC] += 4;
            break;

        case ADDI:
            registers[r1] += imm;
            registers[PC] += 4;
            break;
        case MULI:
            registers[r1] *= imm;
            registers[PC] += 4;
            break;
        case DIVI:
            registers[r1] /= imm;
            registers[PC] += 4;
            break;
        case LI:
            registers[r1] = imm;
            registers[PC] += 4;
            break;

        case JR:
            registers[PC] = registers[r1];
            break;
        case PUSH:
            ((int32_t*)memspace)[registers[SP]] = registers[r1];
            registers[SP] -= 4;
            registers[PC] += 4;
            break;
        case POP:
            registers[r1] = ((int32_t*)memspace)[registers[SP]];
            registers[SP] += 4;
            registers[PC] += 4;
            break;

        case J:
            registers[PC] = imm;
            break;
        case JS:
            registers[PC] += imm;
            break;
        case JZ:
            if (registers[Z]) {
                registers[PC] = imm;
                registers[Z] = 0;
            }
            else {
                registers[PC] += 4;
            }
            break;
        case JZS:
            if (registers[Z]) {
                registers[PC] += imm;
                registers[Z] = 0;
            }
            else {
                registers[PC] += 4;
            }
            break;
        case CALL:
            /* Push return address to stack */
            ((int32_t*)memspace)[registers[SP]] = registers[PC] + 1;
            /* Save stack pointer to return address in frame pointer. */
            registers[SP] -= 4;
            registers[PC] = imm;
            break;
        case PUSHI:
            ((int32_t*)memspace)[registers[SP]] = imm;
            registers[SP] -= 4;
            registers[PC] += 4;
            break;

    }
}


