#ifndef EXECUTION_H
#define EXECUTION_H

#include "tokens.h"

/**
 * @brief Converts the command from struct list to a char **
 *
 * @param l
 * @return char**
 */
char **token_list_to_char_array(struct list *l);

/**
 * @brief Executes a command
 *
 * @param args
 * @return int
 */
int run_command(struct major *mj, char **args);

#endif