#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "instruction.h"
#include "binary-file-format.h"
#include "helpers.h"

#include "lexer.h"
#include "parser.h"
#include "token-list.h"
#include "program-builder.h"

int usage(void) {
    fputs("usage: assembler ifile [-o ofile]\n", stderr);
    return -1;
}

int error(char *str) {
    fputs(str, stderr);
    return usage();
}

char *gen_ofile_name(char *ifile) {
    char *ofile, *tmp = strdup(ifile);
    char *dot = strrchr(tmp, '.');
    if (dot) {
        /* "chop" off rest of string after last '.' */
        *dot = 0;
    }
    ofile = str_cat(2, tmp, ".bin");
    free(tmp);
    return ofile;
}

void assemble(token_list_t *list, char *ofile) {
    program_info_t *prog_info = make_program_info(list->length);
    generate_symbols(prog_info, list);
    token_t *tk = list->head;
    while (tk) {
        if (-1 == compile_token(prog_info, &tk)) { fputs("There were errors, aborting.\n", stderr); return; }
    }
    binary_file_t *bf = calloc(1, sizeof *bf);
    /* Copy the data section. */
    bf->data_section_len = prog_info->data_len;
    bf->data_section = calloc(bf->data_section_len, sizeof *(bf->data_section));
    memcpy(bf->data_section, prog_info->data, bf->data_section_len * sizeof *(bf->data_section));

    /* Copy the text section. */
    bf->text_section_len = prog_info->text_len;
    bf->text_section = calloc(bf->text_section_len, sizeof *(bf->text_section));
    memcpy(bf->text_section, prog_info->text, bf->text_section_len * sizeof *(bf->text_section));

    write_binary_file(ofile, bf);
    free_binary_file(&bf);
    free_program_info(&prog_info);
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
                ofile = strdup(argv[i+1]);
                ++i;
                continue;
            }
            else {
                return error("No output file specified with '-o' flag.\n");
            }
        }
        else {
            ifile = strdup(argv[i]);
        }
    }

    if (ifile == NULL) {
        return error("No input file specified.\n");
    }
    if (ofile == NULL) {
        ofile = gen_ofile_name(ifile);
    }
    else {
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
    if (pass_two) {
        assemble(pass_two, ofile);
    }
    free_token_list(&pass_two);
    free_token_list(&tk_list);
    free(file_contents);
    free(ofile);
    free(ifile);
    return 0;
}
