#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <err.h>
#include <stddef.h>
#include <stdlib.h>

#include "major.h"

/**
 * @brief Frees major and crashes
 *
 * @param err_code
 * @param mj
 * @param message
 */
void my_err(int err_code, struct major *mj, char *message);

/**
 * @brief Displays an error message and returns
 *
 * @param mj
 * @param err_code
 * @param message
 */
int my_soft_err(struct major *mj, int err_code, char *message);

#endif /* UTILS_H */