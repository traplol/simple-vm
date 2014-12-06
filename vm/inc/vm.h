#ifndef _PROJECTS_SIMPLE_VM_VM_H
#define _PROJECTS_SIMPLE_VM_VM_H

/* These values are arbitrary and will be change in the future. */
#define MEMSIZE (64000)
#define TEXT_SECTION_START (200)
#define TEXT_SECTION_SIZE (2500)
#define DATA_SECTION_START (TEXT_SECTION_START + TEXT_SECTION_SIZE + 1)
#define DATA_SECTION_SIZE (8000)
#define HEAP_STACK_SECTION_START (DATA_SECTION_START + DATA_SECTION_SIZE + 1)
#define HEAP_STACK_SECTION_STOP (MEMSIZE - 1)

/* Dumps the register state to stdout. */
void dump_registers(void);

/* Dumps the text section of memory to stdout. */
void dump_text_section(void);

/* Dumps the data section of memory to stdout. */
void dump_data_section(void);

/* Loads the program into the text section of the vm's memory. */
void load_program(unsigned int *program, size_t program_size);

/* Loads the data into the data section of the vm's memory. */
void load_data(char *data, size_t data_size);

/* Resets the state of the vm. */
void init(void);

/* Begins running the program loaded into the vm's memory. */
void run(void);

/* Executes one instruction */
void execute(unsigned int ins);

#endif

