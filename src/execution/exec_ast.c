#include "exec_ast.h"

#include "ast.h"
#include "command_execution.h"
#include "execution.h"
#include "if_execution.h"
#include "parser.h"
#include "pipe.h"
#include "redir.h"
//#include "variables_substitution.h"

static int redir_execution(struct major *mj, struct ast *ast, struct token *tk)
{
    if (tk->word == WORD_PIPE)
    {
        mj->rvalue = exec_pipe(mj, ast);
        return mj->rvalue;
    }
    else if (WORD_PIPE < tk->word && tk->word < WORD_COMMAND)
    {
        mj->rvalue = exec_redir(mj, ast);
        return mj->rvalue;
    }
    else
        return -1;
}

static int conditional_execution(struct major *mj, struct ast *ast,
                                 struct token *tk, int err)
{
    if (tk->word == WORD_IF)
    {
        if (!err)
            exec_ast(mj, ast->right);
        else if (ast->middle)
            exec_ast(mj, ast->middle);
        mj->rvalue = 0;
        return 0;
    }
    else if (tk->word == WORD_WHILE || tk->word == WORD_UNTIL)
    {
        while ((tk->word == WORD_UNTIL) ^ (!err))
        {
            exec_ast(mj, ast->right);
            err = exec_ast(mj, ast->left);
        }
        mj->rvalue = 0;
        return 0;
    }
    else
        return 1;
}

int exec_ast(struct major *mj, struct ast *ast)
{
    if (!ast)
        return 0;
    int err = 0;
    struct token *tk = ast->data;
    //tk->data = variables_substitution(mj, tk->data);
    if (redir_execution(mj, ast, tk) == -1)
        err = exec_ast(mj, ast->left);
    else
        return mj->rvalue;

    if (!conditional_execution(mj, ast, tk, err))
        return 0;
    else if (tk->word == WORD_COMMAND)
        return execution_command(mj, tk);
    else if (tk->word == WORD_AND)
        return !(!err && (!exec_ast(mj, ast->right)));
    else if (tk->word == WORD_OR)
        return !(!err || (!exec_ast(mj, ast->right)));
    else if (tk->word == WORD_SUPERAND)
        return exec_ast(mj, ast->right);
    else if (tk->word == WORD_FOR)
        return exec_for(mj, ast);
    else if (tk->word == WORD_FUNCTION)
        return add_to_funclist(mj, ast);
    return err;
}

int exec_for(struct major *mj, struct ast *ast)
{
    int rvalue = 0;
    if (!ast->middle)
        return 0;
    struct token *start = ast->middle->data;
    for (size_t i = 0; i < start->data->size; i++)
    {
        rvalue = exec_ast(mj, ast->right);
    }
    mj->rvalue = 0;
    return rvalue;
}