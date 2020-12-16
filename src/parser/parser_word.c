#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "list.h"
#include "my_xmalloc.h"
#include "parser.h"
#include "stack.h"
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

static struct token *init_token_with_data(struct major *mj, struct list *list)
{
    struct token *newtoken = my_xcalloc(mj, 1, sizeof(struct token));
    newtoken->data = list;
    newtoken->word = WORD_COMMAND;

    return newtoken;
}

static struct ast *handle_redirs(struct major *mj, struct stack *q,
                                 struct ast *ast)
{
    struct ast *redir = NULL;
    while ((redir = stack_pop(mj, &q)))
    {
        redir->right = ast;
        ast = redir;
    }
    return ast;
}

struct ast *build_command(struct major *mj, struct token **tk,
                          struct token *tk2, struct ast *ast)
{
    struct list *list = add_to_list(mj, NULL, (*tk)->data->head->data);
    token_free(*tk);
    struct stack *redir_stack = NULL;
    *tk = tk2;

    while ((*tk)->data)
    {
        if (is_operator(*tk)) // Redir is the only operator with data
        {
            struct ast *redir = parser_redir(mj, tk);
            redir_stack = stack_add(mj, redir_stack, redir);
            continue;
        }
        list = add_to_list(mj, list, (*tk)->data->head->data);
        token_free(*tk);
        *tk = get_next_token(mj);
    }

    struct token *newtoken = init_token_with_data(mj, list);
    ast = take_action(mj, ast, &newtoken);
    // ^^^ To transform the WORD_COMMAND into an AST
    ast = handle_redirs(mj, redir_stack, ast);

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
