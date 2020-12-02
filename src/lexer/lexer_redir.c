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