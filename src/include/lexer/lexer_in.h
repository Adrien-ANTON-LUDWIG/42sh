#ifndef LEXER_IN_H_
#define LEXER_IN_H_

#include "structures.h"
#include "tokens.h"
#include "lexer_utils.h"

int next_is_in(struct major *mj);
struct token *lexer_in(struct major *mj, struct token *tk, char *variable_name);

#endif /* LEXER_IN_H */