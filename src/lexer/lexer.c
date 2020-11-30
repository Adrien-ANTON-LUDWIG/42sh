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

    int i = token_get(word);
    tk->word = i;

    if (tk->word == WORD_COMMAND)
    {
        struct list *tmp = list_append(mj, tk->data, word);
        tk->data = tmp;
        lexe_cmd(mj, tk);
        return tk;
    }
    if (tk->word == WORD_REDIR)
    {
        tk->redirection = init_redirection(mj);
        char *file = get_word(mj);
        set_redirection(mj, tk->redirection, word, file);        
    }

    free(word);

    return tk;
}

struct token *lexer_build(struct major *mj)
{
    struct custom_FILE *file = mj->file;

    if (!file)
        my_errx(1, mj, "lexer_build: file should not be NULL");

    int from_file = file->fd != CUSTOM_FD;
    char *s = NULL;

    if (from_file && (!file->str || file->lexer_index >= file->len - 1))
    {
        if (!file->str)
            file->str = my_xmalloc(mj, SIZE_TO_GET);

        char *tmp;
        tmp = custom_fgets(file->str, SIZE_TO_GET, file);
        if (!tmp)
        {
            free(file->str);
            file->str = NULL;
            return get_token(mj);
        }
            
        if (file->str)
            file->len = strlen(file->str);
        else
            file->len = 0;
        file->lexer_index = 0;
    }
    else if (!from_file && !file->str)
    {
        s = my_xmalloc(mj, SIZE_TO_GET);
        s = custom_fgets(s, SIZE_TO_GET, file);
    }

    return get_token(mj);
}