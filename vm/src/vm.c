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

void pprint(char *oper, int32_t num) {
    printf("%s=%d", oper, num);
    int32_t *stack_addr;
    int32_t offs;
    if (50000 < num && num < MEMSIZE) {
        stack_addr = ((int32_t*)(memspace + num));
        printf("\n");
        for (int i = 5; i > 0; --i) {
            offs = i * sizeof(int32_t);
            if (num+offs < MEMSIZE) {
                printf(" - <stack@%d.$sp:+%d=%d>\n", num+offs, offs, *(stack_addr+i));
            }
        }
        for (int i = 0; i >= -5; --i) {
            offs = i * sizeof(int32_t);
            printf(" - <stack@%d.$sp:%d=%d>\n", num+offs, offs, *(stack_addr+i));
        }
    }
    printf("\n");
}
void debug_disasm(int32_t ins) {
    instruction_t *instr = disassemble_instruction(ins);
    puts("===========================");
    puts(instr->disassembled_str);
    switch (instr->type) {
        default:
        case NO_OPERANDS:
            break;
        case REGISTER_REGISTER:
            pprint("oper1", registers[instr->operand1.reg]);
            pprint("oper2", registers[instr->operand2.reg]);
            break;
        case REGISTER_IMMEDIATE:
            pprint("oper1", registers[instr->operand1.reg]);
            pprint("oper2", instr->operand2.imm);
            break;
        case REGISTER_NO_IMMEDIATE:
            pprint("oper1", registers[instr->operand1.imm]);
            break;
        case IMMEDIATE_NO_REGISTER:
            pprint("oper1", instr->operand1.imm);
            break;
    }
    free_instruction(&instr);
}

void run(void) {
    size_t pc; 
    int32_t ins;
    printf("%d\n", *((int32_t*)(memspace+registers[PC])));
    while (program_halted == 0) {
        pc = registers[PC];
        ins = *((int32_t*)(memspace+pc));
        debug_disasm(ins);
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
    int32_t imm16, imm21;

    op = get_opcode(ins);
    r1 = get_r1(ins);
    r2 = get_r2(ins);
    imm21 = get_imm21(ins);
    imm16 = get_imm16(ins);


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
            registers[SP] += 4;
            registers[PC] = *((int32_t*)(memspace + registers[SP]));
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
            registers[r1] = *((int32_t*)(imm16 + memspace + registers[r2]));
            registers[PC] += 4;
            break;
        case SW:
            *((int32_t*)(imm16 + memspace + registers[r2])) = registers[r1];
            registers[PC] += 4;
            break;

        case ADDI:
            registers[r1] += imm21;
            registers[PC] += 4;
            break;
        case MULI:
            registers[r1] *= imm21;
            registers[PC] += 4;
            break;
        case DIVI:
            registers[r1] /= imm21;
            registers[PC] += 4;
            break;
        case LI:
            registers[r1] = imm21;
            registers[PC] += 4;
            break;

        case JR:
            registers[PC] = registers[r1];
            break;
        case PUSH:
            *((int32_t*)(memspace + registers[SP])) = registers[r1];
            registers[SP] -= 4;
            registers[PC] += 4;
            break;
        case POP:
            registers[SP] += 4;
            registers[r1] = *((int32_t*)(memspace + registers[SP]));
            registers[PC] += 4;
            break;

        case J:
            registers[PC] = imm21;
            break;
        case JS:
            registers[PC] += imm21;
            break;
        case JZ:
            if (registers[Z]) {
                registers[PC] = imm21;
                registers[Z] = 0;
            }
            else {
                registers[PC] += 4;
            }
            break;
        case JZS:
            if (registers[Z]) {
                registers[PC] += imm21;
                registers[Z] = 0;
            }
            else {
                registers[PC] += 4;
            }
            break;
        case CALL:
            /* Push return address to stack */
            *((int32_t*)(memspace + registers[SP])) = registers[PC] + 4;
            registers[SP] -= 4;
            registers[PC] = imm21;
            break;
        case PUSHI:
            *((int32_t*)(memspace + registers[SP])) = imm21;
            registers[SP] -= 4;
            registers[PC] += 4;
            break;

    }
}


