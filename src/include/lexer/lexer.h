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

struct token_list
{
    struct token *tk;
    struct token_list *next;
    struct token_list *prev;
};

struct lexer
{
    struct token_list *head;
    struct token_list *tail;
};

/**
 * @brief Init a new struct lexer
 *
 * @param mj
 * @return struct lexer*
 */
struct lexer *lexer_init(struct major *mj);

/**
 * @brief Append a token tk to the lexer lex
 *
 * @param mj
 * @param lex
 * @param tk
 */
void lexer_append(struct major *mj, struct lexer *lex, struct token *tk);

/**
 * @brief Pop and return the first token of the lexer lex list
 *
 * @param mj
 * @param lex
 * @return struct token*
 */
struct token *lexer_pop_head(struct major *mj, struct lexer *lex);

/**
 * @brief Frees the lexer lex
 *
 * @param lex
 */
void lexer_free(struct lexer *lex);

/**
 * @brief Called by the parser. Pops the first token from the file in struct
 * major.
 *
 * @param mj
 * @return struct token*
 */
struct token *get_next_token(struct major *mj);

#endif /* LEXER_H */