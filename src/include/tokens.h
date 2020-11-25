#ifndef TOKENS_H
#define TOKENS_H

#include "my_utils.h"

#define TOKENS_STRINGS                                                         \
    {                                                                          \
        "if", "then", "fi"                                                     \
    }

/*
enum tokens
{
    TK_OPERATOR,
    TK_WORD
};

enum operators
{
    OP_EOF
    // OP_DBQUOTE,
    // OP_SGQUOTE,
    // OP_LOG_AND,
    // OP_BIT_AND,
    // OP_LOG_OR,
    // OP_BIT_OR,
    // OP_BSLASH,
    // OP_SLASH,
    // OP_OP_PARENT,
    // OP_CL_PARENT,
    // OP_OP_BRACKET,
    // OP_CL_BRACKET,
    // OP_DOLLAR,
    // OP_SHARP,
    // OP_STAR,
    // OP_ALTGR7
};

enum words
{
    WORD_IF,
    WORD_THEN,
    // WORD_ELSE,
    // WORD_ELIF,
    WORD_FI,
    // WORD_DO,
    // WORD_DONE,
    WORD_COMMAND
};
*/

enum words
{
    WORD_IF,
    WORD_THEN,
    WORD_FI,
    WORD_COMMAND,
    WORD_EOF
};

struct token
{
    enum words word;
    struct list *data;
};

/*
struct word
{
    enum words word;
    // TODO data
};

struct token
{
    enum tokens tk;
    union
    {
        enum operators op;
        struct word word;
    } data;
};
*/

struct token *token_init(struct major *major);

#endif /* TOKENS_H */