#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(void) {

    char *test = to_lower("AbCCCzdEF");
    puts(test);
    free(test);

    return 0;
}
