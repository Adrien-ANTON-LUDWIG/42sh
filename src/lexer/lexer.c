#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "custom_descriptor.h"
#include "my_xmalloc.h"
#include "redirection.h"
#include "tokens.h"

#define BASIC_SEPARATOR "\r\v\n\t "
#define COMMAND_SEPARTOR ";\n\t"

/*
static void _lexer_build(struct major *mj, struct lexer *lex, char *s)
{
    int is_command = 0;
    struct token *tk = NULL;
    while (*s)
    {
        if (*s == ';')
            my_err(1, mj, "-c: erreur de syntaxe près du symbole inattendu ;");

        char *word = get_word(s);
        if (is_command)
        {
            list_append(mj, tk->data, word);
            if (*s == ';' || *s == '\n' || *s == '>')
            {
                is_command = 0;
            }
        }
        else
        {
            tk = token_init(mj);
            int token = token_get(word);
            tk->word = token;
            if (token == WORD_REDIR)
            {
                tk->redirection = init_redirection(mj);
                char *file = get_word(&s);
                set_redirection(mj, tk->redirection, word, file);
            }
            else if (token == WORD_COMMAND)
            {
                if (*s != ';')
                    is_command = 1;
                struct list *tmp = list_append(mj, tk->data, word);
                tk->data = tmp;
            }
            else
            {
                free(word);
            }

            lexer_append(mj, lex, tk);
        }
        if (*s == ';')
            s++;
        while (*s == '\n')
            s++;
    }
}
*/

static struct token *get_token(struct major *mj)
{
    struct token *tk = token_init(mj);
    tk->word = WORD_EOF;

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
    }
    else if (tk->word == WORD_REDIR)
    {
        tk->redirection = init_redirection(mj);
        char *file = get_word(mj);
        set_redirection(mj, tk->redirection, word, file);
    }

    return tk;
}

struct token *lexer_build(struct major *mj)
{
    struct custom_FILE *file = mj->file;

    if (!file)
        return NULL;

    int from_file = file->fd != CUSTOM_FD;
    char *s = NULL;

    if (from_file && (!file->str || file->lexer_index >= file->len - 1))
    {
        if (!file->str)
            file->str = my_xmalloc(mj, SIZE_TO_GET);

        file->str = custom_fgets(file->str, SIZE_TO_GET, file);
        file->len = strlen(file->str);
        file->lexer_index = 0;
    }
    else if (!from_file)
    {
        s = my_xmalloc(mj, sizeof(char) * SIZE_TO_GET);
        s = custom_fgets(s, SIZE_TO_GET, file);
    }

    return get_token(mj);
}