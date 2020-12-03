#include "exec_ast.h"

#include "ast.h"
#include "command_execution.h"
#include "execution.h"
#include "if_execution.h"
#include "parser.h"

int exec_ast(struct major *mj, struct ast *ast)
{
    if (!ast)
        return 0;

    int err = exec_ast(mj, ast->left);

    struct token *tk = ast->data;

    if (tk->word == WORD_IF)
    {
        if (!err)
            exec_ast(mj, ast->right);
        else if (ast->middle)
            exec_ast(mj, ast->middle);
    }
    else if (tk->word == WORD_WHILE || tk->word == WORD_UNTIL)
    {
        while ((tk->word == WORD_UNTIL) ^ (!err))
        {
            exec_ast(mj, ast->right);
            err = exec_ast(mj, ast->left);
        }
    }
    else if (tk->word == WORD_COMMAND)
        return execution_command(mj, tk);
    else if (tk->word == WORD_AND)
        return exec_ast(mj, ast->right);
    else if (tk->word == WORD_FOR)
        return exec_for(mj, ast);
    return err;
}

int exec_for(struct major *mj, struct ast *ast)
{
    int rvalue = 0;
    struct token *start = ast->left->data;
    for (size_t i = 0; i < start->data->size - 1; i++)
    {
        rvalue = exec_ast(mj, ast->right);
    }
    return rvalue;
}