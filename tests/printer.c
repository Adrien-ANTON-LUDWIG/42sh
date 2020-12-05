#include <stdio.h>

#include "lexer.h"
#include "tokens.h"

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
    else if (tk->word == WORD_IN)
        print_token_in(tk);

    printf("\n");
}

void lexer_printer(struct lexer *lex)
{
    if (!lex)
    {
        printf("lexer_printer: Nothing to print\n");
        return;
    }

    struct token_list *tmp = lex->head;

    while (tmp)
    {
        struct token *tk = tmp->tk;
        print_token(tk);
        tmp = tmp->next;
    }
}
