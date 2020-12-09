#include "lexer.h"
#include "tokens.h"

struct token *token_renew(struct major *mj, struct token *tk, int skipline)
{
    token_free(tk);
    tk = get_next_token(mj);
    if (skipline && (tk->word == WORD_NEWLINE || tk->word == WORD_SEMIC))
        return token_renew(mj, tk, 0);
    return tk;
}