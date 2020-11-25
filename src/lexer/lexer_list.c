#include <stdlib.h>

#include "lexer.h"

/**
 * @brief Initialises an instance of struct token_list, used by the lexer to
 * create a double linked list of tokens.
 *
 * @param mj Main structure of our 42sh project.
 * @param tk struct token already initialised and completed
 * @return struct token_list*
 */
static struct token_list *token_list_init(struct major *mj, struct token *tk)
{
    struct token_list *new = malloc(sizeof(struct token_list));

    if (!new)
        my_err(1, mj, "token_list_init: malloc failed");

    new->tk = tk;
    new->next = NULL;
    new->prev = NULL;

    return new;
}

struct lexer *lexer_init(struct major *mj)
{
    struct lexer *new = malloc(sizeof(struct lexer));

    if (!new)
        my_err(1, mj, "lexer_init: malloc failed");

    new->head = NULL;
    new->head = NULL;

    return new;
}

void lexer_append(struct major *mj, struct lexer *lex, struct token *tk)
{
    if (!lex || !tk)
        my_err(1, mj, "lexer_append: lex and tk should not be NULL");

    struct token_list *tk_list = token_list_init(mj, tk);

    if (!lex->head)
    {
        lex->head = tk_list;
        lex->tail = tk_list;
    }
    else
    {
        lex->tail->next = tk_list;
        tk_list->prev = lex->tail;
        lex->tail = tk_list;
    }
}

struct token *lexer_pop_head(struct major *mj, struct lexer *lex)
{
    if (!lex)
        my_err(1, mj, "lexer_pop_head: lex should not be NULL");

    if (!lex->head)
        return NULL;

    struct token_list *to_free = lex->head;
    struct token *tk = to_free->tk;

    if (lex->head == lex->tail)
    {
        lex->head = NULL;
        lex->tail = NULL;
    }
    else
    {
        lex->head = lex->head->next;
        lex->head->prev = NULL;
    }

    free(to_free);

    return tk;
}

static void token_list_free(struct token_list *tk_list)
{
    free(tk_list->tk);
    free(tk_list);
}

void lexer_free(struct lexer *lex)
{
    if (!lex)
        return;

    struct token_list *tk_list = lex->head;
    struct token_list *next = NULL;

    while (tk_list)
    {
        next = tk_list->next;
        token_list_free(tk_list);
        tk_list = next;
    }

    free(lex);
}