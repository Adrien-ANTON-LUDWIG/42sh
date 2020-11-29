#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "structures.h"
#include "tokens.h"

struct ast *parser(struct major *mj);
struct ast *take_action(struct major *mj, struct ast *ast, struct token *tk);
struct ast *parser_if(struct major *mj, struct ast *ast, struct token *tk);
struct ast *add_single_command(struct major *mj, struct ast *ast,
                               struct token *tk);

#endif /* PARSER_H */