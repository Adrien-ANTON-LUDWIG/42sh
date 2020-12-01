#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "custom_descriptor.h"
#include "my_utils.h"
#include "my_xmalloc.h"
#include "redirection.h"
#include "tokens.h"

#define BASIC_SEPARATOR "\r\v\n\t "
#define COMMAND_SEPARTOR ";\n\t"

static struct token *get_token(struct major *mj)
{
    struct token *tk = token_init(mj);
    tk->word = WORD_EOF;

    if (!mj->file->str)
        return tk;

    if (mj->file->fd == CUSTOM_FD && mj->file->lexer_index >= mj->file->len - 1)
        return tk;

    char *word = get_word(mj);

    if (!word)
        return tk;

    int i = word_type(word);
    tk->word = i;

    if (tk->word == WORD_COMMAND)
        return lexer_cmd(mj, tk, word);

    if (tk->word == WORD_REDIR)
        return lexer_redir(mj, tk, word);

    free(word);

    return tk;
}

struct token *get_next_token(struct major *mj)
{
    struct custom_FILE *file = mj->file;

    if (!file)
        my_err(1, mj, "get_next_token: file should not be NULL");

    int from_file = file->fd != CUSTOM_FD;
    char *s = NULL;

    if (from_file && (!file->str || file->lexer_index >= file->len - 1))
    {
        if (!file->str)
            file->str = my_xmalloc(mj, BUFFER_SIZE);

        char *tmp = custom_fgets(file->str, BUFFER_SIZE, file);

        if (!tmp)
        {
            free(file->str);
            file->str = NULL;
            return get_token(mj);
        }
        file->len = strlen(file->str);
        file->lexer_index = 0;
    }
    else if (!from_file && !file->str)
    {
        s = my_xmalloc(mj, BUFFER_SIZE);
        s = custom_fgets(s, BUFFER_SIZE, file);
    }

    return get_token(mj);
}