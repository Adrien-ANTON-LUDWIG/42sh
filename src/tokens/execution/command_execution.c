#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "cd.h"
#include "echo.h"
#include "exec_ast.h"
#include "execution.h"
#include "export.h"
#include "major.h"
#include "my_err.h"
#include "source.h"
#include "tokens.h"

struct ast *search_funclist(struct major *mj, char *name)
{
    struct funclist *current = mj->flist;
    while (current)
    {
        if (!(strcmp(current->ast->data->data->head->data, name)))
            return current->ast;
        current = current->next;
    }
    return NULL;
}

int exec_if_known(struct major *mj, char **command)
{
    struct ast *func = search_funclist(mj, *command);

    if (func)
    {
        exec_ast(mj, func->right);
        free(command);
        return 1;
    }

    if (!strcmp(*command, "echo"))
    {
        b_echo(command);
        return 1;
    }

    return 0;
}

int execution_command(struct major *mj, struct token *tk)
{
    if (!tk)
        my_err(1, mj, "execution command: no token found");

    char **command = token_list_to_char_array(tk->data);

    if ((exec_if_known(mj, command)))
        return mj->rvalue;

    mj->rvalue = run_command(mj, command);
    free(command);
    return mj->rvalue;
}

int allow_son_execution_command(struct major *mj, struct token *tk, int rvalue)
{
    if (mj && tk)
        return rvalue;
    return rvalue;
}
