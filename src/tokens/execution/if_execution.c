#include "tokens.h"

/**
 * @brief Standard for node execution
 *
 * @param mj
 * @param tk
 * @return int
 */
int execution_if(struct major *mj, struct token *tk)
{
    if (mj && tk)
        return 0;
    return 0;
}

/**
 * @brief Standard for allowing the left son of a if node/token
 *
 * @param mj
 * @param tk
 * @param rvalue
 * @return int
 */
int allow_son_execution_if(struct major *mj, struct token *tk, int rvalue)
{
    if (mj && tk)
        return rvalue == 0;
    if (rvalue == 0)
        return 1;
    return 0;
}