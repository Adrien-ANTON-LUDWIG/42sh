#ifndef LEXER_H
#define LEXER_H

#define BUFFER_SIZE 512

#include "major.h"
#include "tokens.h"

/**
 * @brief Called by the parser. Pops the first token from the file in struct
 * major.
 *
 * @param mj
 * @return struct token*
 */
struct token *get_next_token(struct major *mj);

#endif /* LEXER_H */