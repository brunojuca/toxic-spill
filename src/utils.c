#include <stdio.h>

int throwInputError()
{
    fprintf(stderr, "Error reading input\n");
    return 1;
}

int throwMallocError()
{
    fprintf(stderr, "Error in memory allocation\n");
    return 1;
}
