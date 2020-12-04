#include "lexer_cmd.h"

#include "lexer_utils.h"
#include "my_utils.h"
#include "redirection.h"
#include "lexer_operator.h"

struct token *lexer_cmd(struct major *mj, struct token *tk, char *cmd)
{
    struct list *tmp = list_append(mj, tk->data, cmd);
    tk->data = tmp;

    if (!mj->file->str)
        return tk;

    if (mj->file->str[mj->file->lexer_index] == ';')
    {
        mj->file->lexer_index++;
        return tk;
    }

    char *word = get_word(mj);

    while (word)
    {
        if (*word == '#')
        {
            free(word);
            return tk;
        }

        list_append(mj, tk->data, word);

        if (next_is_redirection(mj) || next_is_operator(mj))
            break;

        if (!mj->file->str)
            return tk;

        if (mj->file->str[mj->file->lexer_index] == ';')
        {
            mj->file->lexer_index++;
            break;
        }
        word = get_word(mj);
    }

    return tk;
}