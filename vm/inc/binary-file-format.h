#ifndef _PROJECTS_SIMPLE_VM_BINARY_FILE_FORMAT_H
#define _PROJECTS_SIMPLE_VM_BINARY_FILE_FORMAT_H

typedef struct binary_file {
    unsigned long data_section_len;
    char *data_section;
    unsigned long text_section_len;
    unsigned int *text_section;
} binary_file_t;

typedef enum {
    SUCCESS = 0,
    FILE_NOT_FOUND
} binary_read_write_error_t;

/* Writes the binary file to a file, returning a non-zero integer
 * if there was an error. */
int write_binary_file(char *filename, binary_file_t *binary_file);

/* Reads a binary file into a new binary_file_t struct. */
binary_file_t *read_binary_file(char *filename);

/* Frees the data in a binary_file_t. */
void free_binary_file(binary_file_t **binary_file);

#endif

