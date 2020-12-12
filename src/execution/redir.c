#define _POSIX_C_SOURCE 2
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

#include "custom_descriptor.h"
#include "exec_ast.h"
#include "list.h"
#include "my_err.h"

static int run(struct major *mj, int fd_old, int fd_new, struct ast *ast)
{
    int save_old = dup(fd_old);
    if (fd_old == 0)
        fclose(stdin);
    dup2(fd_new, fd_old);
    close(fd_new);
    mj->rvalue = exec_ast(mj, ast);
    dup2(save_old, fd_old);
    close(save_old);
    if (fd_old == 0)
        stdin = fdopen(STDIN_FILENO, "rw");
    return mj->rvalue;
}

static int run_close(struct major *mj, int fd_old, struct ast *ast)
{
    int save_old = dup(fd_old);
    close(fd_old);
    mj->rvalue = exec_ast(mj, ast);
    dup2(save_old, fd_old);
    close(save_old);
    if (fd_old == 0)
        stdin = fdopen(STDIN_FILENO, "rw");
    return mj->rvalue;
}

static int exec_redir_l(struct major *mj, struct ast *ast)
{
    char *out = ast->left->data->data->head->data;
    struct token *tk = ast->data;
    int flags = O_RDONLY;
    if (tk->word == WORD_REDIR_LR)
        flags = O_RDWR;
    int new_fd = open(out, flags, 0644);

    if (new_fd == -1)
        my_err(1, mj, "exec_redir_l: Couldn't open the file !");

    int old_fd = *(tk->data->head->data) - '0';

    return run(mj, old_fd, new_fd, ast->right);
}

static int exec_redir_la(struct major *mj, struct ast *ast)
{
    char *out = ast->left->data->data->head->data;
    size_t len = strlen(out);
    int old_fd = *(ast->data->data->head->data) - '0';

    int is_number = 1;

    for (size_t i = 0; is_number && i < len; i++)
        is_number = '0' <= out[i] && out[i] <= '9';

    if (is_number)
    {
        int n = strtol(out, NULL, 10);
        return run(mj, old_fd, n, ast->right);
    }

    if (!strcmp(out, "-"))
        return run_close(mj, old_fd, ast->right);

    if (ast->data->word == WORD_REDIR_RA)
    {
        int new_fd = open(out, O_TRUNC | O_CREAT | O_WRONLY, 0644);

        if (new_fd == -1)
            my_err(1, mj, "exec_redir: Couldn't open the file !");

        run(mj, old_fd, new_fd, ast->right);
    }

    my_err(1, mj, "exec_redir_la: redirection ambigue");
    return 1;
}

static int exec_redir_r(struct major *mj, struct ast *ast)
{
    char *out = ast->left->data->data->head->data;
    struct token *tk = ast->data;
    int flags = (tk->word == WORD_REDIR_RR) ? O_APPEND : O_TRUNC;
    int new_fd = open(out, flags | O_CREAT | O_WRONLY, 0644);

    if (new_fd == -1)
        my_err(1, mj, "exec_redir: Couldn't open the file !");

    int old_fd = *(tk->data->head->data) - '0';

    return run(mj, old_fd, new_fd, ast->right);
}

static int exec_redir_ll(struct major *mj, struct ast *ast)
{
    if (!mj || !ast)
        return 1;
    return 1;
}

int exec_redir(struct major *mj, struct ast *ast)
{
    struct token *tk = ast->data;
    if (tk->word == WORD_REDIR_L)
        return exec_redir_l(mj, ast);
    else if (tk->word == WORD_REDIR_R || tk->word == WORD_REDIR_RR
             || tk->word == WORD_REDIR_RP)
        return exec_redir_r(mj, ast);
    else if (tk->word == WORD_REDIR_LA || tk->word == WORD_REDIR_RA)
        return exec_redir_la(mj, ast);
    return exec_redir_ll(mj, ast);
}