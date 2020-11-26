#include <stdio.h>

#include "lexer.h"
#include "tokens.h"

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

        tmp = tmp->next;
        printf("\n");
    }
    printf("---\n");
}