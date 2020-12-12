#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "cd.h"
#include "echo.h"
#include "exec_ast.h"
#include "execution.h"
#include "exit.h"
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

    int known = 0;

    if (func)
    {
        exec_ast(mj, func->right);
        known = 1;
    }
    else if (!strcmp(*command, "echo"))
    {
        mj->rvalue = b_echo(command);
        known = 1;
    }
    else if (!strcmp(*command, "cd"))
    {
        mj->rvalue = b_cd(command);
        known = 1;
    }
    else if (!strcmp(*command, "exit"))
    {
        mj->rvalue = b_exit(mj, command);
        known = 1;
    }
    else if (!strcmp(*command, "export"))
    {
        mj->rvalue = b_export(command);
        known = 1;
    }
    /*
        if (!strcmp(*command, "source"))
        {
            mj->rvalue = b_source(command);
            known = 1;
        }
    */
    if (known)
        free(command);

    return known;
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
