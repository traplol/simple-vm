#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "irr.h"
#include "irm.h"
#include "helpers.h"

#define MEMSIZE (64000)
#define TEXT_SECTION_START (200)
#define TEXT_SECTION_SIZE (2500)
#define DATA_SECTION_START (TEXT_SECTION_START + TEXT_SECTION_SIZE + 1)
#define DATA_SECTION_SIZE (8000)
#define HEAP_STACK_SECTION_START (DATA_SECTION_START + DATA_SECTION_SIZE + 1)
#define HEAP_STACK_SECTION_STOP (MEMSIZE - 1)

register_t registers[REGISTER_COUNT];
unsigned int memspace[MEMSIZE];
int program_halted = 0;

void execute(unsigned int ins);

void dump_registers(void) {
    char *str, *imm_str;
    for (int i = 0; i < REGISTER_COUNT; ++i) {
        imm_str = imm_to_str(registers[i], "%d");
        str = str_cat(3, reg_to_str(i), ": ", imm_str);
        puts(str);
        free(str);
        free(imm_str);
    }
}

void load_program(unsigned int *program, size_t program_size) {
    if (program_size > TEXT_SECTION_SIZE) {
        fputs("Program size too big.\n", stderr);
        exit(1);
    }
    for (size_t i = 0; i < program_size; ++i) {
        memspace[TEXT_SECTION_START + i] = program[i];
    }
}

void load_data(char *data, size_t data_size) {
}

void run(void) {
    size_t pc;
    while (program_halted == 0) {
        pc = registers[PC];
        execute(memspace[pc]);
    }
}

void init(void) {
    for (size_t i = 0; i < MEMSIZE; ++i) {
        memspace[i] = 0;
    }
    registers[PC] = TEXT_SECTION_START;
}

int main(void) {
    unsigned int *program = calloc(50, sizeof *program);
    int i = 0;
    /* Simple factorial(12) (Largest signed 32 bit) program. */
    program[i++] = compile_irm(LI, G0, 1);   /* Start */
    program[i++] = compile_irm(LI, G1, 13);  /* End */
    program[i++] = compile_irm(LI, G3, 1);   /* Product */
    program[i++] = compile_irr(CMP, G0, G1); /* Check if G0 = G1. */
    program[i++] = compile_irm(JZS, NUL, 4); /* If compare succeeds, exit loop. */
    program[i++] = compile_irr(MUL, G3, G0); /* Mul the product by counter. */
    program[i++] = compile_irm(ADDI, G0, 1); /* Increment counter. */
    program[i++] = compile_irm(JS, NUL, -4); /* Jump to compare */


    program[i++] = HALT;
    init();
    load_program(program, 50);
    load_data(NULL, 0);
    free(program);
    run();
    dump_registers();
    return 0;
}

void execute(unsigned int ins) {
    int op, r1, r2, imm;

    op = get_opcode(ins);
    r1 = get_r1(ins);
    r2 = get_r2(ins);
    imm = get_imm(ins);


    switch (op) {
        case HALT:
            program_halted = 1;
            break;
        case NOP:
            registers[PC]++;
            break;

        case ADD:
            registers[r1] += registers[r2];
            registers[PC]++;
            break;
        case MUL:
            registers[r1] *= registers[r2];
            registers[PC]++;
            break;
        case DIV:
            registers[r1] /= registers[r2];
            registers[PC]++;
            break;
        case CMP:
            registers[Z] = registers[r1] == registers[r2];
            registers[PC]++;
            break;
        case AND:
            registers[r1] &= registers[r2];
            registers[PC]++;
            break;
        case OR:
            registers[r1] |= registers[r2];
            registers[PC]++;
            break;
        case XOR:
            registers[r1] ^= registers[r2];
            registers[PC]++;
            break;
        case SLL:
            registers[r1] <<= registers[r2];
            registers[PC]++;
            break;
        case SRL:
            registers[r1] >>= registers[r2];
            registers[PC]++;
            break;
        case MOV:
            registers[r1] = registers[r2];
            registers[PC]++;
            break;

        case ADDI:
            registers[r1] += imm;
            registers[PC]++;
            break;
        case MULI:
            registers[r1] *= imm;
            registers[PC]++;
            break;
        case DIVI:
            registers[r1] /= imm;
            registers[PC]++;
            break;
        case LI:
            registers[r1] = imm;
            registers[PC]++;
            break;

        case JR:
            registers[PC] = registers[r1];
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
                registers[PC]++;
            }
            break;
        case JZS:
            if (registers[Z]) {
                registers[PC] += imm;
                registers[Z] = 0;
            }
            else {
                registers[PC]++;
            }
            break;
        case CALL:
            break;
    }
}


