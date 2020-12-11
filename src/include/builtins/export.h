#ifndef EXPORT_H_
#define EXPORT_H_

#include "major.h"

/**
 * @brief Set export attribute for shell variables. The -f option is not handled
 * for now.
 *
 * -f refer to shell functions
 * -n remove the export property from each NAME
 * -p display a list of all exported variables and functions
 *
 * @param mj
 * @param argv First arg should be export
 * @return int
 */
int b_export(char *argv[]);

#endif /* EXPORT_H_ */