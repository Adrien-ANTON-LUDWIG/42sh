#ifndef LEXER_REDIR_H
#define LEXER_REDIR_H

#include "structures.h"

struct token *lexer_redir(struct token *tk, char *word);
int next_is_redirection(struct major *mj);
int is_redir(char *word);

#endif