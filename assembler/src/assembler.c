#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "instruction.h"
#include "binary-file-format.h"
#include "helpers.h"
#include "lexer.h"
#include "parser.h"

int usage(void) {
    fputs("usage: assembler ifile [-o ofile]\n", stderr);
    return -1;
}

int error(char *str) {
    fputs(str, stderr);
    return usage();
}

int main(int argc, char **argv) {
    char *ifile, *ofile, *arg, *file_contents;
    int i;
    size_t file_size;
    FILE *fp;

    --argc; ++argv;
    if (argc == 0) {
        return usage();
    }

    ifile = NULL; ofile = NULL;
    /* Parse aguments. */
    for (i = 0; i < argc; ++i) {
        arg = argv[i];
        if (strcmp(arg, "-o") == 0) {
            if (i + 1 < argc) {
                ofile = argv[i+1];
                ++i;
                continue;
            }
            else {
                return error("No output file specified with '-o' flag.\n");
            }
        }
        else {
            ifile = argv[i];
        }
    }

    if (ifile == NULL) {
        return error("No input file specified.\n");
    }

    fp = fopen(ifile, "rb");
    if (fp == NULL) {
        return error("Input file not found.\n");
    }

    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    file_contents = calloc(1 + file_size, 1);
    fread(file_contents, 1, file_size, fp);
    fclose(fp);

    token_list_t *tk_list = tokenize(file_contents);
    token_list_t *pass_two = parse(tk_list);
    token_t *cur = pass_two->head;
    while (cur) {
        puts(cur->str);
        cur = cur->next;
    }
    free_token_list(&pass_two);
    free_token_list(&tk_list);
    free(file_contents);
    return 0;
}
