#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include "major.h"
#include "tokens.h"

/**
 * @brief Parses and executes
 *
 * @param mj
 * @return struct ast*
 */
void parser(struct major *mj);

/**
 * @brief Decides which function to call depending on the kind of token
 *
 * @param mj
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *take_action(struct major *mj, struct ast *ast, struct token **tk);

/**
 * @brief Adds a AND command with the ast as its left son and tk as its right
 * son
 *
 * @param mj major structure
 * @param ast an ast
 * @param tk
 * @return struct ast* The newly built ast
 */
struct ast *add_single_command(struct major *mj, struct ast *ast,
                               struct token **tk);

/* PARSER_IF */

/**
 * @brief Handles the parsing of an "if" condition
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_if(struct major *mj, struct ast *ast, struct token **tk);

/* PARSER_CPDLIST */
void parser_cpdlist(struct major *mj, struct token **expr, struct ast *newast,
                    int (*should_loop)(enum words));

/**
 * @brief Handles the parsing of an "while" condition
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_while(struct major *mj, struct ast *ast, struct token **tk);

/**
 * @brief Handles the parsing of an "while" condition
 *
 * @param mj major structure
 * @param ast
 * @param tk
 * @return struct ast*
 */
struct ast *parser_for(struct major *mj, struct ast *ast, struct token **tk);
struct ast *parser_operator(struct major *mj, struct ast *ast,
                            struct token **tk);

struct ast *get_ast(struct major *mj, struct ast *ast, struct token **tk);

struct token *build_command(struct major *mj, struct token **tk,
                            struct token *tk2);

struct list *add_to_list(struct major *mj, struct list *list, char *str);

struct ast *parser_function(struct major *mj, struct ast *ast,
                            struct token **tk, struct token *tk2);
struct ast *parser_word(struct major *mj, struct ast *ast, struct token **tk);
int is_operator(struct token *tk);

#endif /* PARSER_H */