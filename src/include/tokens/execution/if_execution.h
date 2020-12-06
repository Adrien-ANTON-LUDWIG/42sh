#ifndef IF_EXECUTION_H
#define IF_EXECUTION_H

#include "major.h"
#include "tokens.h"

/**
 * @brief Standard for node execution
 *
 * @param mj
 * @param tk
 * @return int
 */
int execution_if(struct major *mj, struct token *tk);

/**
 * @brief Standard for allowing the left son of a if node/token
 *
 * @param mj
 * @param tk
 * @param rvalue
 * @return int
 */
int allow_son_execution_if(struct major *mj, struct token *tk, int rvalue);

#endif /* IF_EXECUTION_H */