#include <stdio.h>
#include <string.h>

void ThrowError(char *message, char *file, int line, int critical) {
    printf("%s in %s at line %d", message, file, line);
    if (critical) {
        exit(critical);
    }
}