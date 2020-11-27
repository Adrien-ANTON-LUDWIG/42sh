#include "execution.h"
#include "tokens.h"

int execution_command(struct major *mj, struct token *tk)
{
    if (!tk)
        my_err(1, mj, "execution command: no token found");

    char **command = token_list_to_char_array(tk->data);
    return run_command(command);
}

int allow_son_execution_command(struct major *mj, struct token *tk, int rvalue)
{
    if (mj && tk)
        return rvalue;
    return rvalue;
}
