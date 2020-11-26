#include "parser.h"

#include <stdlib.h>

#include "ast.h"

static struct ast *parse_if(struct major *mj, struct ast *ast,
                            struct lexer *lex, struct token *tk_if)
{
    struct token *cond = lexer_pop_head(mj, lex);
    struct token *then = lexer_pop_head(mj, lex);
    struct token *expr = lexer_pop_head(mj, lex);
    struct token *fi = lexer_pop_head(mj, lex);

    if (then->word != WORD_THEN || fi->word != WORD_FI)
        my_err(1, mj, "parse_if: syntaxe error");

    struct ast *new = ast_init_with_tokens(mj, tk_if, cond, expr);

    ast = ast_add(mj, ast, new);

    return ast;
}

struct ast *parser(struct major *mj, struct lexer *lex)
{
    struct ast *ast;
    struct token *tk = NULL;
    while ((tk = lexer_pop_head(mj, lex)))
    {
        if (tk->word == WORD_IF)
            ast = parse_if(mj, ast, lex, tk);
        else if (tk->word == WORD_COMMAND)
        {
            struct ast *new = ast_init(mj, tk, NULL, NULL);
            ast = ast_add(mj, ast, new);
        }
        else if (tk->word == WORD_EOF)
            return ast;
        else
            my_err(1, mj, "parser: syntaxe error");
    }
    return ast;
}