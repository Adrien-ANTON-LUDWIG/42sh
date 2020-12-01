#ifndef LEXER_REDIR_H
#define LEXER_REDIR_H

#include "redirection.h"

struct token *lexer_redir(struct major *mj, struct token *tk, char *word);

#endif