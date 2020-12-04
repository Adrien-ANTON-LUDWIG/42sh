#include "lexer_redir.h"

#include "custom_descriptor.h"
#include "lexer.h"
#include "lexer_utils.h"
#include "tokens.h"

struct token *lexer_redir(struct major *mj, struct token *tk, char *word)
{
    tk->redirection = init_redirection(mj);
    char *file_redir = get_word(mj);
    set_redirection(mj, tk->redirection, word, file_redir);
    free(word);
    return tk;
}

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