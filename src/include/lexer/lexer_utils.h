#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H

#include "custom_descriptor.h"
#include "structures.h"

/**
 * @brief Returns a word without whitespace before nor after the word
 *
 * @param mj
 * @return char*
 */
char *get_word(struct major *mj);

/**
 * @brief Return a word for the first time
 *
 * @param mj
 * @return char*
 */
char *get_first_word(struct major *mj);

/**
 * @brief Gets the next word if it's a WORD_REDIR.
 * If it is not a WORD_REDIR, return NULL
 *
 * @param mj
 * @return char*
 */
char *get_redir(struct major *mj);

/**
 * @brief Gets the operator if it is one.
 *
 * @param mj
 * @return char* NULL if it is not an operator
 */
char *get_operator(struct major *mj);

#endif /* LEXER_UTILS_H */