#include "globals.h"

void *validate_malloc(size_t size)
{
    void *mem = malloc(size);
    if (!mem)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
    return mem;
}