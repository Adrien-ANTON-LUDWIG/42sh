#ifndef LEXER_OPERATOR_H_
#define LEXER_OPERATOR_H_

#include "major.h"

/**
 * @brief Checks if the next word is a WORD_OPERATOR
 *
 * @param mj
 * @return int
 */
int next_is_operator(struct major *mj);

#endif /* LEXER_OPERATOR_H_ */