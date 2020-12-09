#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "list.h"
#include "my_xmalloc.h"
#include "parser.h"
#include "string_manipulation.h"
#include "tokens.h"

struct list *add_to_list(struct major *mj, struct list *list, char *str)
{
    struct list_item *item = my_xcalloc(mj, 1, sizeof(struct list_item));
    item->data = strdup(str);

    if (list)
    {
        list->tail->next = item;
        list->size += 1;
    }
    else
    {
        list = my_xmalloc(mj, sizeof(struct list));
        list->head = item;
        list->size = 1;
    }

    list->tail = item;
    return list;
}

struct ast *build_command(struct major *mj, struct token **tk,
                          struct token *tk2, struct ast *ast)
{
    struct list *list = add_to_list(mj, NULL, (*tk)->data->head->data);
    token_free(*tk);
    struct token *pending_operator = NULL;
    struct token *pending_file = NULL;
    *tk = tk2;
    while ((*tk)->data)
    {
        if (is_operator(*tk))
        {
            if (pending_operator) // FIXME
            {
                token_free(pending_operator);
                token_free(pending_file);
            }
            pending_operator = *tk;
            pending_file = get_next_token(mj);
            *tk = get_next_token(mj);
            continue;
        }
        list = add_to_list(mj, list, (*tk)->data->head->data);
        token_free(*tk);
        *tk = get_next_token(mj);
    }

    struct token *newtoken = my_xcalloc(mj, 1, sizeof(struct token));
    newtoken->data = list;
    newtoken->word = WORD_COMMAND;
    ast = take_action(mj, ast, &newtoken);
    if (pending_file)
    {
        struct ast *newast = create_ast(mj, pending_operator);
        newast->left = ast;
        newast->right = get_ast(mj, newast->right, &pending_file);
        token_free(pending_file);
        return newast;
    }
    return ast;
}

struct ast *parser_word(struct major *mj, struct ast *ast, struct token **tk)
{
    struct token *tk2 = get_next_token(mj);
    if (tk2->word == WORD_DPARENTHESIS)
        return parser_function(mj, ast, tk, tk2);
    struct ast *command = build_command(mj, tk, tk2, ast);
    return command;
}
