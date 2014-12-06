#include <stdlib.h>
#include "binary-file-format.h"
#include "instruction.h"
#include "opdefs.h"

int main(void) {
    unsigned int *prg = calloc(50, sizeof *prg);
    int i = 0;
    /* factorial(5) */
    prg[i++] = assemble_instruction(LI, G0, 1);  /* Start */
    prg[i++] = assemble_instruction(LI, G1, 5);  /* End */
    prg[i++] = assemble_instruction(LI, G3, 1);  /* Product */
    prg[i++] = assemble_instruction(GT, G0, G1);
    prg[i++] = assemble_instruction(JZS, 4, 0);  /* If G0 > G1, exit loop */
    prg[i++] = assemble_instruction(MUL, G3, G0);/* Mul product and counter */
    prg[i++] = assemble_instruction(ADDI, G0, 1);/* Increment counter */
    prg[i++] = assemble_instruction(JS, -4, 0);  /* Jump to compare */
    prg[i++] = assemble_instruction(HALT, 0, 0); /* Halt */

    binary_file_t bf;
    bf.text_section_len = i;
    bf.text_section = prg;
    bf.data_section_len = 0;
    bf.data_section = NULL;

    write_binary_file("factorial.bin", &bf);

    free(prg);
    return 0;
}
