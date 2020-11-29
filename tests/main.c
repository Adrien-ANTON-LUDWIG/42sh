#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

#include "argument_handler.h"
#include "ast.h"
#include "custom_descriptor.h"
#include "lexer.h"
#include "parser.h"
#include "printer.h"

TestSuite(_42sh, .timeout = 15);

Test(_42sh, custom_descriptor1)
{
    struct custom_FILE *f = createfrom_string(
        "Salut!\nJe suis un test\nEt je te veux du mal\nissou");
    char *buffer = malloc(128 * sizeof(char));
    char *savedbuffer = buffer;
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "Salut!\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "Je suis un test\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "Et je te veux du mal\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_expect_eq(strcmp(buffer, "issou"), 0);
    char *newbuffer = custom_fgets(buffer, 128, f);
    cr_expect_null(newbuffer);
    free(savedbuffer);
    custom_fclose(f);
}

Test(_42sh, merge_commands)
{
    char *argv[] = { "Salut", "comment", "ça", "va" };
    char *merged = merge_arguments(4, argv);
    cr_assert_eq(strcmp("Salut comment ça va", merged), 0);
    free(merged);
}

Test(_42sh, merge_commands_first_empty)
{
    char *argv[] = { "", "Salut" };
    char *merged = merge_arguments(2, argv);
    cr_assert_eq(strcmp("Salut", merged), 0);
    free(merged);
}

Test(_42sh, merge_commands_empty)
{
    char *argv[] = { NULL };
    char *merged = merge_arguments(0, argv);
    cr_assert_null(merged);
    free(merged);
}

Test(_42sh, lexer_one_simple_line)
{
    struct lexer *lex = lexer_build(NULL, "echo test");

    cr_assert_not_null(lex, "NULL");
    cr_assert_eq(lex->head->tk->word, WORD_COMMAND, "Not WORD_COMMAND");
    cr_assert_eq(strcmp(lex->head->tk->data->head->data, "echo"), 0,
                 "First string not 'echo'");
    cr_assert_eq(strcmp(lex->head->tk->data->head->next->data, "test"), 0,
                 "Second string not 'test'");
    cr_assert_eq(lex->tail->tk->word, WORD_EOF, "No WORD_EOF");

    lexer_free(lex);
}

Test(_42sh, lexer_one_line_if)
{
    struct lexer *lex = lexer_build(NULL, "if echo test; then echo toto; fi");

    cr_assert_not_null(lex, "NULL");
    cr_assert_eq(lex->head->tk->word, WORD_IF, "Not WORD_IF");
    cr_assert_eq(lex->head->next->tk->word, WORD_COMMAND, "Not WORD_COMMAND_1");
    cr_assert_eq(strcmp(lex->head->next->tk->data->head->data, "echo"), 0,
                 "Not echo 1");
    cr_assert_eq(strcmp(lex->head->next->tk->data->head->next->data, "test"), 0,
                 "Not test");
    cr_assert_eq(lex->head->next->next->tk->word, WORD_THEN, "Not WORD_THEN");
    cr_assert_eq(lex->head->next->next->next->tk->word, WORD_COMMAND,
                 "Not WORD_COMMAND_2");
    cr_assert_eq(
        strcmp(lex->head->next->next->next->tk->data->head->data, "echo"), 0,
        "Not echo 1");
    cr_assert_eq(
        strcmp(lex->head->next->next->next->tk->data->head->next->data, "toto"),
        0, "Not toto");
    cr_assert_eq(lex->head->next->next->next->next->tk->word, WORD_FI,
                 "Not WORD_FI");
    cr_assert_eq(lex->tail->tk->word, WORD_EOF, "No WORD_EOF");

    lexer_free(lex);
}

Test(_42sh, lexer_simple_redir_append)
{
    struct lexer *lex = lexer_build(NULL, "echo test >> file");
    lexer_free(lex);
}

Test(_42sh, parser_simple_if)
{
    struct lexer *lex = lexer_build(NULL, "if echo test; then echo toto; fi");
    struct ast *ast = take_action(NULL, NULL, lex, lexer_pop_head(NULL, lex));
    cr_assert_eq(strcmp(ast->left->data->data->head->data, "echo"), 0);
    cr_assert_eq(strcmp(ast->left->data->data->tail->data, "test"), 0);
    cr_assert_eq(strcmp(ast->right->data->data->head->data, "echo"), 0);
    cr_assert_eq(strcmp(ast->right->data->data->tail->data, "toto"), 0);
    lexer_free(lex);
    ast_free(ast);
}

int main(int argc, char **argv)
{
    struct criterion_test_set *tests = criterion_initialize();

    int result = 0;
    if (criterion_handle_args(argc, argv, true))
        result = !criterion_run_all_tests(tests);
    criterion_finalize(tests);
    return result;
}
