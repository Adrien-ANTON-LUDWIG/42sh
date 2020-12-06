#ifndef COMMAND_EXECUTION_H
#define COMMAND_EXECUTION_H

#include "major.h"
#include "tokens.h"

/**
 * @brief Standard for node execution
 *
 * @param mj
 * @param tk
 * @return int
 */
int execution_command(struct major *mj, struct token *tk);

/**
 * @brief Standard for allowing the left son of a command node/token
 *
 * @param mj
 * @param tk
 * @param rvalue
 * @return int
 */
int allow_son_execution_command(struct major *mj, struct token *tk, int rvalue);

#endif /* COMMAND_EXECUTION_H */