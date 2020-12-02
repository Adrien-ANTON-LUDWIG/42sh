#include "execution.h"
#include "tokens.h"

/**
 * @brief Standard for node execution
 *
 * @param mj
 * @param tk
 * @return int
 */
int execution_command(struct major *mj, struct token *tk)
{
    if (!tk)
        my_err(1, mj, "execution command: no token found");

    char **command = token_list_to_char_array(tk->data);
    int rvalue = run_command(command);
    free(command);
    mj->rvalue = rvalue;
    return rvalue;
}

/**
 * @brief Standard for allowing the left son of a command node/token
 *
 * @param mj
 * @param tk
 * @param rvalue
 * @return int
 */
int allow_son_execution_command(struct major *mj, struct token *tk, int rvalue)
{
    if (mj && tk)
        return rvalue;
    return rvalue;
}
