#include <stdio.h>
#include <string.h>

typedef enum bool {
    false,
    true
} bool;

void ThrowError(char *message, char *file, int line, bool critical) {
    printf("%s in %s at line %d\n", message, file, line);
    if (critical) {
        exit(critical);
    }
}