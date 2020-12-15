#include "command_execution.h"

#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "cd.h"
#include "char_array_op.h"
#include "echo.h"
#include "exec_ast.h"
#include "execution.h"
#include "exit.h"
#include "export.h"
#include "my_err.h"
#include "shopt.h"
#include "variables_substitution.h"

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
        return 1;
    }
    else if (!strcmp(*command, "echo"))
    {
        mj->rvalue = b_echo(command);
        return 1;
    }
    else if (!strcmp(*command, "cd"))
    {
        mj->rvalue = b_cd(command);
        return 1;
    }
    else if (!strcmp(*command, "exit"))
    {
        mj->rvalue = b_exit(mj, command);
        return 1;
    }
    else if (!strcmp(*command, "export"))
    {
        mj->rvalue = b_export(command);
        return 1;
    }
    else if (!strcmp(*command, "shopt"))
    {
        mj->rvalue = b_shopt_options(mj, command);
        return 1;
    }

    return 0;
}

int execution_command(struct major *mj, struct token *tk)
{
    if (!tk)
        my_err(1, mj, "execution command: no token found");
    char **argv = variables_substitution(mj, tk->data);
    if (!(exec_if_known(mj, argv)))
        mj->rvalue = run_command(mj, argv);
    char_array_free(argv);
    return mj->rvalue;
}

int allow_son_execution_command(struct major *mj, struct token *tk, int rvalue)
{
    if (mj && tk)
        return rvalue;
    return rvalue;
}
