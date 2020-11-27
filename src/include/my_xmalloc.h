#ifndef MY_XMALLOC_H
#define MY_XMALLOC_H

#include <stddef.h>

void *my_xmalloc(struct major *mj, size_t size);
void *my_xcalloc(struct major *mj, size_t nbmem, size_t size);

#endif