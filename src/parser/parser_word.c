#include <stdlib.h>

#include "ast.h"
#include "list.h"
#include "my_xmalloc.h"
#include "parser.h"
#include "string_manipulation.h"
#include "tokens.h"

struct list *add_to_list(struct major *mj, struct list *list, char *str)
{
    struct list_item *item = my_xcalloc(mj, 1, sizeof(struct list_item));
    item->data = str;

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

struct token *build_command(struct major *mj, struct token **tk,
                            struct token *tk2)
{
    struct list *list = add_to_list(mj, NULL, (*tk)->data->head->data);
    list = add_to_list(mj, list, tk2->data->head->data);
    free(*tk);
    free(tk2);
    *tk = get_next_token(mj);

    while (is_not_in(*((*tk)->data->head->data), IS_OPERATOR))
    {
        list = add_to_list(mj, list, (*tk)->data->head->data);
        free(tk);
        *tk = get_next_token(mj);
    }

    struct token *newtoken = my_xcalloc(mj, 1, sizeof(struct token));
    newtoken->data = list;
    newtoken->word = WORD_COMMAND;
    return newtoken;
}

struct ast *parser_word(struct major *mj, struct ast *ast, struct token *tk)
{
    struct token *tk2 = get_next_token(mj);
    if (tk2->word == WORD_DPARENTHESIS)
        return parser_function(mj, ast, tk, tk2);
    struct token *command = build_command(mj, &tk, tk2);
    ast = take_action(mj, ast, command);
    return ast;
}
