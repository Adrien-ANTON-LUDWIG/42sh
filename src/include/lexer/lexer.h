#ifndef NEWLEXER_H
#define NEWLEXER_H

#include "major.h"

/**
 * @brief Get the word token
 *
 *
 *
 * @param mj
 * @param tk
 * @return struct token*
 */
struct token *get_next_token(struct major *mj);

#endif