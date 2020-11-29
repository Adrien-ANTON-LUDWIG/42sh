#include "lexer_cmd.h"

#include "lexer_utils.h"
#include "my_utils.h"
#include "redirection.h"

struct token *lexe_cmd(struct major *mj, struct token *tk)
{
    if (!mj->file->str)
        return tk;

    if (mj->file->str[mj->file->lexer_index] == ';')
    {
        mj->file->lexer_index++;
        return tk;
    }

    char *word = get_word(mj);

    while (word && !is_redirection(mj))
    {
        list_append(mj, tk->data, word);

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