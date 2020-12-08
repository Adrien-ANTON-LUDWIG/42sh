#ifndef NEWLEXER_H
#define NEWLEXER_H

#include "major.h"

/**
 * @brief Get the redirection token
 *
 * @param mj
 * @param c First chevron
 * @return struct token*
 */
struct token *get_token_redir(struct major *mj, char c);

/**
 * @brief Get the operator token
 *
 * @param mj
 * @return struct token*
 */
struct token *get_token_operator(struct major *mj);

/**
 * @brief Get the word token
 *
 *
 *
 * @param mj
 * @param tk
 * @return struct token*
 */
struct token *get_token_word(struct major *mj, struct token *tk);
struct token *get_next_token(struct major *mj);

#endif