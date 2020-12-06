#include "printer.h"

#include <stdio.h>

#include "list.h"

static void print_token_in(struct token *tk)
{
    struct list_item *l = tk->data->head;

    if (!l)
        return;
    printf(" %s in", l->data);

    l = l->next;

    while (l)
    {
        printf(" %s", l->data);
        l = l->next;
    }
    printf("\n");
}

static void print_token_redir(struct token *tk)
{
    printf(" ");
    if (tk->data)
        printf("%s", tk->data->head->data);
}

void print_token(struct token *tk)
{
    printf("%s", token2string(tk));

    if (tk->word == WORD_COMMAND)
    {
        struct list_item *li = tk->data->head;
        for (size_t i = 0; i < tk->data->size; i++)
        {
            printf(" %s", li->data);
            li = li->next;
        }
    }
    else if (tk->word >= WORD_REDIR_LR && tk->word <= WORD_REDIR_R)
        print_token_redir(tk);
    else if (tk->word == WORD_IN)
        print_token_in(tk);

    printf("\n");
}