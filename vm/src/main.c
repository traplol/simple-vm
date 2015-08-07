#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vm.h"
#include "binary-file-format.h"

int disasm_flag = 0, dump_data_flag = 0, dump_text_flag = 0;


int main(int argc, char **argv) {
    --argc; ++argv;
    char *usage_str =
        "usage: \n"
        "\tvm [-d|--disassemble] [-D|--dump-data] [-T|--dump-text] file\n";
    if (!argc) {
        fputs(usage_str, stderr);
        return -1;
    }
    char *filename;
    for (int i = 0; i < argc; ++i) {
        char *arg = argv[i];
        if (strcmp(arg, "-d") == 0 || strcmp(arg, "--disassemble") == 0) {
            disasm_flag = 1;
        }
        else if (strcmp(arg, "-D") == 0 || strcmp(arg, "--dump-data") == 0) {
            dump_data_flag = 1;
        }
        else if (strcmp(arg, "-T") == 0 || strcmp(arg, "--dump-text") == 0) {
            dump_text_flag = 1;
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

    init();
    load_program(r->text_section, r->text_section_len);
    load_data(r->data_section, r->data_section_len);
    if (dump_data_flag) {
        dump_data_section();
    }
    if (dump_text_flag) {
        dump_text_section();
    }
    if (disasm_flag) {
        disassemble_program(r->text_section, r->text_section_len);
    }
    run();
#ifdef DEBUG_DISASSEM
    dump_registers();
#endif
    return 0;

}
