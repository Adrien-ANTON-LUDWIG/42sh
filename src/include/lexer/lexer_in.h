#ifndef LEXER_IN_H_
#define LEXER_IN_H_

#include "major.h"
#include "tokens.h"

/**
 * @brief Checks if next word is a 'in' word
 *
 * @param mj
 * @return int
 */
int next_is_in(struct major *mj);

/**
 * @brief Lexes a WORD_IN
 *
 * @param mj
 * @param tk token IN
 * @param variable_name name of the variable which can be found before the 'in'
 * word
 * @return struct token*
 */
struct token *lexer_in(struct major *mj, struct token *tk, char *variable_name);

#endif /* LEXER_IN_H */