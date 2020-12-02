#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "structures.h"
#include "tokens.h"

struct ast *parser(struct major *mj);
struct ast *take_action(struct major *mj, struct ast *ast, struct token *tk);
struct ast *add_single_command(struct major *mj, struct ast *ast,
                               struct token *tk);

/* PARSER_IF */
struct ast *parser_if(struct major *mj, struct ast *ast, struct token *tk);

/* PARSER_CPDLIST */
void parser_cpdlist(struct major *mj, struct token **expr, struct ast *newast,
                    int (*should_loop)(enum words));

struct ast *parser_while(struct major *mj, struct ast *ast, struct token *tk);

#endif /* PARSER_H */