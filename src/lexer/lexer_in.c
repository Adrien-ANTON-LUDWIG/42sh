#include "lexer_in.h"

#include <stddef.h>
#include <string.h>

struct token *lexer_in(struct major *mj, struct token *tk, char *variable_name)
{

    struct list *tmp = list_append(mj, tk->data, variable_name);
    tk->data = tmp;
    tk->word = WORD_IN;

    char *word = get_word(mj);

    while (word)
    {
        if (*word == ';' || *word == '\n')
        {
            free(word);
            break;
        }
        list_append(mj, tk->data, word);

        if (!mj->file->str)
            return tk;

        if (mj->file->str[mj->file->lexer_index] == ';'
            || mj->file->str[mj->file->lexer_index] == '\n')
        {
            mj->file->lexer_index++;
            break;
        }
        word = get_word(mj);
    }
    return tk;
}

int next_is_in(struct major *mj)
{
    size_t temp = mj->file->lexer_index;
    char *s = get_word(mj);

    if (!s)
        return 0;

    int return_value = (strcmp(s, "in") == 0);

    free(s);

    if (!return_value)
        mj->file->lexer_index = temp;

    return return_value;
}