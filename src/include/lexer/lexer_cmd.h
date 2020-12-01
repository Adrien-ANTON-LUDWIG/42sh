#ifndef LEXER_CMD_H
#define LEXER_CMD_H

#include "lexer.h"
#include "my_utils.h"

/**
 * @brief Returns a token WORD_COMMAND tk with it's own list of commands
 *
 * @param mj
 * @param tk
 * @return struct token*
 */
struct token *lexer_cmd(struct major *mj, struct token *tk, char *cmd);

#endif