#ifndef _PROJECTS_SIMPLE_VM_VM_H
#define _PROJECTS_SIMPLE_VM_VM_H

#include <stdint.h>

/* These values are arbitrary and will be change in the future. */
#define MEMSIZE (64000)
#define TEXT_SECTION_START (200)
#define TEXT_SECTION_SIZE (2500)
#define DATA_SECTION_START (TEXT_SECTION_START + TEXT_SECTION_SIZE + 1)
#define DATA_SECTION_SIZE (8000)
#define HEAP_STACK_SECTION_START (DATA_SECTION_START + DATA_SECTION_SIZE + 1)
#define HEAP_STACK_SECTION_STOP (MEMSIZE - 1)
#define STACK_START HEAP_STACK_SECTION_STOP

/* Prints the disassembly of the program. */
void disassemble_program(unsigned char *program, size_t size);

/* Dumps the register state to stdout. */
void dump_registers(void);

/* Dumps the text section of memory to stdout. */
void dump_text_section(void);

/* Dumps the data section of memory to stdout. */
void dump_data_section(void);

/* Loads the program into the text section of the vm's memory. */
void load_program(unsigned char *program, size_t size);

/* Loads the data into the data section of the vm's memory. */
void load_data(unsigned char *data, size_t size);

/* Resets the state of the vm. */
void init(void);

/* Begins running the program loaded into the vm's memory. */
void run(void);

/* Executes one instruction */
void execute(int32_t ins);

#endif

