#include <string.h>
#include <stdlib.h>
#include "smart-compile.h"

typedef struct instruction {
    opcode_t opcode;
    register_t r1;
    union {
        register_t r2;
        int imm;
    } operand2 ;
    unsigned int compiled;
    char *raw;
} instruction_t;

char *to_lower(char *str) {
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

opcode_t str_get_opcode(char *op) {
}

instruction_t compile(char *str_ins) {
    instruction_t ins;

    strcpy(ins.raw, str_ins);
    return ins;
}

int main(void) {

    char *test = to_lower("AbCCCzdEF");
    puts(test);
    free(test);

    return 0;
}
