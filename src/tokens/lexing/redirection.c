#include "redirection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "custom_descriptor.h"
#include "lexer_utils.h"
#include "structures.h"
#include "tokens.h"

int next_is_redirection(struct major *mj)
{
    size_t temp = mj->file->lexer_index;
    char *str[] = TOKENS_STRINGS_REDIR;
    int return_value = 0;

    char *s = get_word(mj);

    if (!s)
        return 0;

    for (int i = 0; i < 2; i++)
    {
        if (strcmp(s, str[i]) == 0)
        {
            return_value = 1;
            break;
        }
    }
    free(s);
    mj->file->lexer_index = temp;
    return return_value;
}

struct redir *init_redirection(struct major *mj)
{
    struct redir *new_redir = malloc(sizeof(struct redir));

    if (!new_redir)
        my_err(1, mj, "init_redirection: bad malloc");

    new_redir->std_out_append_mode = 0;
    new_redir->std_err_append_mode = 0;
    new_redir->std_err = NULL;
    new_redir->std_in = NULL;
    new_redir->std_out = NULL;

    return new_redir;
}

void set_redirection(struct major *mj, struct redir *redirection, char *word,
                     char *file)
{
    if (!word || !file)
        my_err(2, mj, "set_rediction: syntax error near unexpected token `;");

    if (!strcmp(">", word))
    {
        redirection->std_out = file;
        redirection->std_out_append_mode = REDIR_TRUNK;
    }
    else if (!strcmp(">>", word))
    {
        redirection->std_out = file;
        redirection->std_out_append_mode = REDIR_APPEND;
    }
}

void free_redirection(struct redir *redir)
{
    if (!redir)
        return;

    free(redir->std_in);
    free(redir->std_out);
    free(redir->std_err);
    free(redir);
}