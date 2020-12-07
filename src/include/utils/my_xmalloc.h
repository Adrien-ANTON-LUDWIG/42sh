#ifndef MY_XMALLOC_H_
#define MY_XMALLOC_H_

#include <stddef.h>

#include "major.h"
#include "my_xmalloc.h"

/**
 * @brief Malloc but crashes if malloc fails
 *
 * @param mj
 * @param size
 * @return void*
 */
void *my_xmalloc(struct major *mj, size_t size);

/**
 * @brief realloc but crashes if realloc fails
 *
 * @param mj
 * @param ptr
 * @param size
 * @return void*
 */
void *my_xrealloc(struct major *mj, void *ptr, size_t size);

/**
 * @brief Calloc but crashes on fail
 *
 * @param mj
 * @param nbmem
 * @param size
 * @return void*
 */
void *my_xcalloc(struct major *mj, size_t nbmem, size_t size);

#endif /* MY_XMALLOC_H_ */