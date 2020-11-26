#include <stdio.h>

#include "lexer.h"
#include "tokens.h"

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
    else if (tk->word == WORD_REDIR)
    {
        printf("\n\tstdin: %s\n", tk->redirection->std_in);
        printf("\tstdout: %s\tappend_mode: %d\n", tk->redirection->std_out,
                tk->redirection->append_mode);
        printf("\tstderr: %s\n", tk->redirection->std_err);
    }
    
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
    printf("---\n");
}
