#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "binary-file-format.h"

int disasm_flag = 0;

int main(int argc, char **argv) {
    --argc; ++argv;
    if (!argc) {
        fputs("usage: vm [-d|--disassemble] file\n", stderr);
        return -1;
    }
    char *filename;
    for (int i = 0; i < argc; ++i) {
        char *arg = argv[i];
        if (strcmp(arg, "-d") == 0 || strcmp(arg, "--disassemble") == 0) {
            disasm_flag = 1;
        }
        else {
            filename = arg;
        }
    }

    binary_file_t *r = read_binary_file(filename);
    if (r == NULL) {
        fprintf(stderr, "File '%s' not found.\n", filename);
        return -1;
    }

    if (disasm_flag) {
        disassemble_program(r->text_section, r->text_section_len);
    }
    else {
        init();
        load_program(r->text_section, r->text_section_len);
        load_data(r->data_section, r->data_section_len);
        run();
        dump_registers();
    }

    return 0;

}
