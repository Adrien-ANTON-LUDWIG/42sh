#ifndef TOKENS_H
#define TOKENS_H

#include "major.h"

#define TOKENS_STRINGS                                                         \
    {                                                                          \
        "if", "then", "elif", "else", "fi", "while", "until", "for", "in",     \
            "do", "done", "function", "{", "}"                                 \
    }

#define TOKENS_STRINGS_REDIR                                                   \
    {                                                                          \
        "<>", ">|", "<&", ">&", "<<-", "<<", ">>", "<", ">"                    \
    }

#define DEFAULT_REDIR_VALUE                                                    \
    {                                                                          \
        '0', '1', '0', '1', '0', '0', '1', '0', '1'                            \
    }

#define NUMBER_OF_REDIR 9

/**
 * @brief Enum words
 */
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
    WORD_FUNCTION,
    WORD_LBRACKET,
    WORD_RBRACKET,
    WORD_AND,
    WORD_OR,
    WORD_PIPE,
    WORD_REDIR_LR,
    WORD_REDIR_RP,
    WORD_REDIR_LA,
    WORD_REDIR_RA,
    WORD_REDIR_LLM,
    WORD_REDIR_LL,
    WORD_REDIR_RR,
    WORD_REDIR_L,
    WORD_REDIR_R,
    WORD_COMMAND,
    WORD_EOF,
    WORD_SUPERAND,
    WORD_SEMIC,
    WORD_NEWLINE,
    WORD_LPARENTHESIS,
    WORD_RPARENTHESIS,
    WORD_DPARENTHESIS,
    WORD_WORD,
    WORD_ASSIGNMENT,
    WORD_DSEMIC
};

/**
 * @brief The structures that the lexer inits in order to give it to the parser.
 *
 */
struct token
{
    enum words word;
    struct list *data;
};

/**
 * @brief Inits a struct token.
 *
 * @param major
 * @return struct token*
 */
struct token *token_init(struct major *major, enum words word);

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
int word_type(struct major *mj, struct token *tk, char *s);

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
struct token *token_renew(struct major *mj, struct token *tk, int skipline);

#endif /* TOKENS_H */