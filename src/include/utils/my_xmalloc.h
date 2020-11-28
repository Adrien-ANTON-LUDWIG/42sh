#ifndef MY_XMALLOC_H
#define MY_XMALLOC_H

#include <stddef.h>

#include "my_utils.h"

/**
 * @brief malloc but crashes if malloc fails
 *
 * @param mj
 * @param size
 * @return void*
 */
void *my_xmalloc(struct major *mj, size_t size);

/**
 * @brief Calloc but crashes on fail
 *
 * @param mj
 * @param nbmem
 * @param size
 * @return void*
 */
void *my_xcalloc(struct major *mj, size_t nbmem, size_t size);

#endif