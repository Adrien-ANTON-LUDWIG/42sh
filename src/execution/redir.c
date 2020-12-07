#include "redir.h"

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "exec_ast.h"
#include "list.h"
#include "my_err.h"

static int run_command(struct major *mj, int fd_new, int fd_old,
                       struct ast *ast)
{
    int rvalue = exec_ast(mj, ast);
    fflush(stdout);
    close(fd_old);
    close(fd_new);
    return rvalue;
}

static int run(struct major *mj, int fd_old, int fd_new, struct ast *ast)
{
    int old_dup = dup(fd_old);
    dup2(fd_new, fd_old);
    int pid = fork();

    if (!pid)
        exit(run_command(mj, fd_new, fd_old, ast));

    int returnvalue = 0;

    waitpid(pid, &returnvalue, 0);

    returnvalue = WEXITSTATUS(returnvalue);

    fflush(stdout);
    dup2(old_dup, fd_old);
    close(old_dup);
    close(fd_new);
    return returnvalue;
}

int exec_redir(struct major *mj, struct ast *ast)
{
    char *out = ast->right->data->data->head->data;
    struct token *tk = ast->data;
    int flags = (tk->word == WORD_REDIR_RR) ? O_APPEND : O_TRUNC;
    int new_fd = open(out, flags | O_CREAT | O_WRONLY,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (new_fd == -1)
        my_err(1, mj, "exec_redir: Couldn't open the file !");

    int old_fd = *(tk->data->head->data) - '0';

    return run(mj, old_fd, new_fd, ast->left);
}