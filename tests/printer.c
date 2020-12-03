#include <stdio.h>

#include "lexer.h"
#include "tokens.h"

static void print_token_redir(struct token *tk)
{
    if (tk->redirection->std_in)
        printf(" < %s", tk->redirection->std_in);
    if (tk->redirection->std_out)
        printf(" 1%s %s",
               tk->redirection->std_out_append_mode == REDIR_TRUNK ? ">" : ">>",
               tk->redirection->std_out);
    if (tk->redirection->std_err)
        printf(" 2%s %s",
               tk->redirection->std_err_append_mode == REDIR_TRUNK ? ">" : ">>",
               tk->redirection->std_err);
}

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
    else if (tk->word == WORD_REDIR)
    {
        print_token_redir(tk);
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
