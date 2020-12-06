#ifndef LEXER_H
#define LEXER_H

#define BUFFER_SIZE 512

#include <stddef.h>
#include <string.h>

#include "lexer_cmd.h"
#include "lexer_redir.h"
#include "lexer_utils.h"
#include "my_utils.h"
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