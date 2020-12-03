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
        if (allow_son_execution_if(mj, tk, err))
            exec_ast(mj, ast->right);
        else if (ast->middle)
            exec_ast(mj, ast->middle);
    }
    else if (tk->word == WORD_WHILE || tk->word == WORD_UNTIL)
    {
        while ((tk->word == WORD_UNTIL) ^ allow_son_execution_if(mj, tk, err))
        {
            exec_ast(mj, ast->right);
            err = exec_ast(mj, ast->left);
        }
    }
    else if (tk->word == WORD_COMMAND)
        return execution_command(mj, tk);
    else if (tk->word == WORD_AND)
        return exec_ast(mj, ast->right);

    return err;
}