#ifndef LEXER_REDIR_H
#define LEXER_REDIR_H

#include "major.h"
#include "tokens.h"

/**
 * @brief Lexes a WORD_REDIR
 *
 * @param tk
 * @param word
 * @return struct token* correctly set with the type of the redir and
 * the filedescriptor
 */
struct token *lexer_redir(struct token *tk, char *word);

/**
 * @brief Checks if the next word is a WORD_REDIRECTION_X
 *
 * @param mj
 * @return int
 */
int next_is_redirection(struct major *mj);

/**
 * @brief Checks if the word is a redir. Returns 1 if true, 0 otherwise.
 *
 * @param word
 * @return int
 */
int is_redir(char *word);

#endif /* LEXER_REDIR_H */