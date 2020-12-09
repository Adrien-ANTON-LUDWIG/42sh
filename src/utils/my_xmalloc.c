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

/**
 * @brief realloc but crashes if realloc fails
 *
 * @param mj
 * @param ptr
 * @param size
 * @return void*
 */
void *my_xrealloc(struct major *mj, void *ptr, size_t size)
{
    ptr = realloc(ptr, size);
    if (!ptr)
        my_err(1, mj, "realloc failed");
    return ptr;
}

/**
 * @brief Calloc but crashes on fail
 *
 * @param mj
 * @param nbmem
 * @param size
 * @return void*
 */
void *my_xcalloc(struct major *mj, size_t nbmem, size_t size)
{
    void *ptr = calloc(nbmem, size);
    if (!ptr)
        my_err(1, mj, "calloc failed");
    return ptr;
}