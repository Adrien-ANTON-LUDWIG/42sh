#ifndef TOKENS_H
#define TOKENS_H

#include "my_utils.h"

#define TOKENS_STRINGS                                                         \
    {                                                                          \
        "if", "then", "elif", "else", "fi", "while", "until", "for", "in",     \
            "do", "done", "&&", "||", "|"                                      \
    }

#define TOKENS_STRINGS_REDIR                                                   \
    {                                                                          \
        ">", ">>"                                                              \
    }

#define REDIR_APPEND 1
#define REDIR_TRUNK 0

enum words
{
    WORD_IF,
    WORD_THEN,
    WORD_ELIF,
    WORD_ELSE,
    WORD_FI,
    WORD_WHILE,
    WORD_UNTIL,
    WORD_FOR,
    WORD_IN,
    WORD_DO,
    WORD_DONE,
    WORD_AND,
    WORD_OR,
    WORD_REDIR,
    WORD_PIPE,
    WORD_COMMAND,
    WORD_EOF,
    WORD_SUPERAND,
};

struct token
{
    enum words word;
    struct list *data;
    struct redir *redirection;
};

struct redir
{
    char *std_in;
    char *std_out;
    char *std_err;
    int std_out_append_mode;
    int std_err_append_mode;
};

/**
 * @brief Inits a struct token.
 *
 * @param major
 * @return struct token*
 */
struct token *token_init(struct major *major);

/**
 * @brief Create a deepcopy of the given src token.
 *
 * @param mj
 * @param src
 * @return struct token*
 */
struct token *token_cpy(struct major *mj, struct token *src);

/**
 * @brief Gets the WORD_TYPE (enum words) of a char*
 *
 * @param s
 * @return int
 */
int word_type(char *s);

/**
 * @brief Converts a the field word of a token into a string
 *
 * @param tk
 * @return char*
 */
char *token2string(struct token *tk);

/**
 * @brief Frees a token struct.
 *
 * @param tk Token to free
 */
void token_free(struct token *tk);

#endif /* TOKENS_H */