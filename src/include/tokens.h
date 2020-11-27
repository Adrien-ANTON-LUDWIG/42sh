#ifndef TOKENS_H
#define TOKENS_H

#include "my_utils.h"

#define TOKENS_STRINGS                                                         \
    {                                                                          \
        "if", "then", "fi"                                                     \
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
    WORD_FI,
    WORD_REDIR,
    WORD_COMMAND,
    WORD_EOF,
    WORD_AND
};

struct token
{
    enum words word;
    struct list *data;
    struct redir *redirection;
};

struct redir // 2> filename
{
    char *std_in; // defaut = NULL
    char *std_out; // defaut = NULL
    char *std_err; // defaut = filename
    int std_out_append_mode;
    int std_err_append_mode;
};

struct token *token_init(struct major *major);
int token_get(char *s);
char *token2string(struct token *tk);
void token_free(struct token *tk);

#endif /* TOKENS_H */