#include "list.h"
#include "major.h"
#include "my_err.h"
#include "parser.h"
#include "tokens.h"

static int should_loop(enum words word)
{
    return word != WORD_DSEMIC && word != WORD_ESAC;
}

static void parser_case_clause(struct major *mj, struct ast *ast,
                               struct token **tk)
{
    if ((*tk)->word == WORD_ESAC)
        return;

    if ((*tk)->word == WORD_LPARENTHESIS)
        *tk = token_renew(mj, *tk, 0);

    while ((*tk)->word != WORD_RPARENTHESIS || !ast->left)
    {
        if (!((*tk)->word <= WORD_RBRACKET || (*tk)->word == WORD_WORD
              || (*tk)->word == WORD_ASSIGNMENT))
            my_err(2, mj, "parser_case_clause: not a valid word");

        if (!ast->left)
        {
            ast->left = create_ast(mj, *tk);
            *tk = get_next_token(mj);
        }
        else
        {
            list_append(mj, ast->left->data->data, (*tk)->data->head->data);
            (*tk)->data->head->data = NULL;
            *tk = token_renew(mj, *tk, 0);
        }

        if ((*tk)->word == WORD_PIPE)
            *tk = token_renew(mj, *tk, 0);
        else if ((*tk)->word != WORD_RPARENTHESIS)
            my_err(2, mj, "parser_case_clause: expected '|' or ')");
    }

    *tk = token_renew(mj, *tk, 1);
    parser_cpdlist(mj, tk, ast, should_loop);

    if ((*tk)->word == WORD_DSEMIC)
    {
        *tk = token_renew(mj, *tk, 0);
        struct ast *newast = create_ast(mj, token_cpy(mj, ast->data));
        ast->middle = newast;
        parser_case_clause(mj, newast, tk);
    }
}

struct ast *parser_case(struct major *mj, struct ast *ast, struct token **tk)
{
    superand_creator(mj, &ast);

    struct ast *newast = create_ast(mj, *tk);
    *tk = get_next_token(mj);

    if (!((*tk)->word <= WORD_RBRACKET || (*tk)->word == WORD_WORD
          || (*tk)->word == WORD_ASSIGNMENT))
        my_err(2, mj, "parser_case: invalid word");

    list_append(mj, newast->data->data, (*tk)->data->head->data);
    (*tk)->data->head->data = NULL;
    (*tk) = token_renew(mj, *tk, 1);

    if ((*tk)->word != WORD_IN)
        my_err(2, mj, "parser_case: missing 'in'");

    *tk = token_renew(mj, *tk, 1);

    parser_case_clause(mj, newast, tk);

    if ((*tk)->word != WORD_ESAC)
        my_err(2, mj, "Lol noob, you forgot the 'Esac', what a loser");

    *tk = token_renew(mj, *tk, 1);

    if (ast)
        ast->right = newast;
    else
        ast = newast;
    return ast;
}