#include "list.h"
#include "major.h"
#include "parser.h"
#include "tokens.h"

static int should_loop(enum words word)
{
    return word != WORD_DSEMIC && word != WORD_ESAC;
}

static void parser_case_clause(struct major *mj, struct ast *ast,
                               struct token **tk)
{
    // TODO Make token_cpy a deep copy
    struct ast *newast = create_ast(mj, token_cpy(mj, ast->data));
    ast->middle = newast;

    if ((*tk)->word == WORD_LPARENTHESIS)
        *tk = token_renew(mj, *tk, 0);

    while ((*tk)->word != WORD_RPARENTHESIS)
    {
        if (!((*tk)->word < WORD_LBRACKET || (*tk)->word == WORD_WORD))
            my_err(2, mj, "parser_case_clause: not a valid word");

        if (!newast->left)
        {
            newast->left = create_ast(mj, *tk);
            *tk = get_next_token(mj);
        }
        else
        {
            list_append(mj, newast->left->data, (*tk)->data->head->data);
            (*tk)->data->head->data = NULL;
            *tk = token_renew(mj, *tk, 0);
        }

        if ('|')
            token_renew else(!')') my_err
    }

    passer les newlines->token renew

        parser_cpdlist(mj, newast->middle)

            if (!WORD_DSEMIC) return
}

struct ast *parser_case(struct major *mj, struct ast *ast, struct token **tk)
{
    superand_creator(mj, &ast);

    struct ast *newast = create_ast(mj, *tk);
    *tk = get_next_token(mj);

    if (!((*tk)->word < WORD_LBRACKET || (*tk)->word == WORD_WORD))
        my_err(2, mj, "parser_case: invalid word");

    list_append(mj, newast->data->data, (*tk)->data);
    *tk = NULL;
    (*tk) = token_renew(mj, *tk, 1);

    if ((*tk)->word != WORD_IN)
        my_err(2, mj, "parser_case: missing 'in'");

    *tk = NULL;
    *tk = token_renew(mj, *tk, 1);

    if ((*tk)->word == WORD_ESAC)
        return else parser_case_clause(mj, newast, tk);

    // gérer esac

    return ast;
}