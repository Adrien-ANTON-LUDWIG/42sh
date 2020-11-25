#ifndef UTILS_H
#define UTILS_H

#include <err.h>
#include <stdlib.h>

#include "structures.h"

/* MAJOR */
void major_init(void);
void major_free(struct major *mj);

/* MY_ERR */
void my_err(int err_code, struct major *mj, char *message);

#endif /* UTILS_H */