#include "my_xmalloc.h"

#include <stddef.h>
#include <stdlib.h>

#include "my_err.h"

void *my_xmalloc(struct major *mj, size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        my_err(1, mj, "malloc failed");
    return ptr;
}

void *my_xcalloc(struct major *mj, size_t nbmem, size_t size)
{
    void *ptr = calloc(nbmem, size);
    if (!ptr)
        my_err(1, mj, "calloc failed");
    return ptr;
}