#include <stdio.h>
#include <stdlib.h>

#include "binary-file-format.h"


/* Writes the binary file to a file. */
int write_binary_file(char *filename, binary_file_t *binary_file) {
    FILE *fp = fopen(filename, "wb");
    unsigned long ds_len = binary_file->data_section_len;
    unsigned long ts_len = binary_file->text_section_len;
    unsigned char *ds = binary_file->data_section;
    unsigned char *ts = binary_file->text_section;
    /* Data section length */
    fwrite(&ds_len, sizeof ds_len, 1, fp);
    /* Text section length */
    fwrite(&ts_len, sizeof ts_len, 1, fp);
    /* Data section */
    fwrite(ds, sizeof *ds, ds_len, fp);
    /* Text section */
    fwrite(ts, sizeof *ts, ts_len, fp);
    fclose(fp);
    return SUCCESS;
}

/* Reads a binary file into a new binary_file_t struct. */
binary_file_t *read_binary_file(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }
    size_t file_size;
    fseek(fp, 0L, SEEK_END);
    file_size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned long ds_len = 0, ts_len = 0;
    unsigned char *ds;
    unsigned char *ts;

    fread(&ds_len, sizeof ds_len, 1, fp);
    fread(&ts_len, sizeof ts_len, 1, fp);

    if (ds_len > 0) {
        ds = malloc(ds_len * sizeof *ds);
        fread(ds, sizeof *ds, ds_len, fp);
    }
    else {
        ds = NULL;
    }
    if (ts_len > 0) {
        ts = malloc(ts_len * sizeof *ts);
        fread(ts, sizeof *ts, ts_len, fp);
    }
    else {
        ts = NULL;
    }

    fclose(fp);

    binary_file_t *bf = malloc(sizeof *bf);
    bf->data_section_len = ds_len;
    bf->text_section_len = ts_len;
    bf->data_section = ds;
    bf->text_section = ts;
    return bf;
}

/* Frees the data in a binary_file_t. */
void free_binary_file(binary_file_t **binary_file) {
    if (binary_file && *binary_file) {
        free((*binary_file)->data_section);
        free((*binary_file)->text_section);
        free(*binary_file);
        *binary_file = NULL;
    }
}

