#ifndef ECHO_H_
#define ECHO_H_

#include "major.h"

/**
 * @brief Echo builtin. Returns 0.
 * Three options are possible : -n, -e and -E.
 *
 * -n do not output the trailing newline
 * -e enable interpretation of backslash escapes
 * -E disable interpretation of backslash escapes (default)
 *
 * @param argv First arg has to be "echo"
 * @return int
 */
int b_echo(char **argv);

#endif /* ECHO_H_ */