#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H

#include "custom_descriptor.h"
#include "structures.h"

/**
 * @brief While classifier returns 1, increments 1 to the lexer_index in
 * mj->file
 *
 * @param classifier
 * @param mj
 */
void skip_class(int (*classifier)(int), struct major *mj);

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

#endif