#include "pipe.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec_ast.h"
#include "execution.h"
#include "my_err.h"

static int run_programs(struct major *mj, int pipefd[2], struct ast *left,
                        struct ast *right)
{
    int pid[2];
    if (!(pid[0] = fork()))
    {
        fclose(stdin);
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        exit(exec_ast(mj, right));
    }
    else
    {
        if (!(pid[1] = fork()))
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            exit(exec_ast(mj, left));
        }
        else
        {
            int rvalue = 0;
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(pid[1], NULL, 0);
            waitpid(pid[0], &rvalue, 0);
            return rvalue;
        }
    }
    return 0;
}

int exec_pipe(struct major *mj, struct ast *ast)
{
    if (!ast->left || !ast->right)
        my_err(2, mj, "Unexpected EOF");
    int pipefd[2];
    if (pipe(pipefd) == -1)
        my_err(1, mj, "Failed to create pipe");
    int rvalue = run_programs(mj, pipefd, ast->left, ast->right);

    return WEXITSTATUS(rvalue);
}
