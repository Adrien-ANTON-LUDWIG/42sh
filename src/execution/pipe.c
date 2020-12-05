#include "pipe.h"

#include <err.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "exec_ast.h"
#include "execution.h"

static int run_programs(struct major *mj, int pipefd[2], struct ast *left,
                        struct ast *right)
{
    int pid[2];
    pid[0] = fork();
    if (!pid[0])
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        exec_ast(mj, right);
        exit(127);
    }
    else
    {
        pid[1] = fork();
        if (!pid[1])
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            exec_ast(mj, left);
            exit(127);
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
    int pipefd[2];
    if (pipe(pipefd) == -1)
        my_err(1, mj, "Failed to create pipe");
    int rvalue = run_programs(mj, pipefd, ast->left, ast->right);

    return WEXITSTATUS(rvalue);
}
