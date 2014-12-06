#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

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
        if (strcmp(arg, "-d") == 0 || strcmp(arg, "--disasseble") == 0) {
            disasm_flag = 1;
        }
        else {
            filename = arg;
        }
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fputs("File does not exist\n", stderr);
        return -1;
    }

    size_t file_size;
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char *data = malloc(file_size * sizeof *data);
    fread(data, sizeof *data, file_size, fp);
    fclose(fp);
    return 0;
}
