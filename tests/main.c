#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

#include "custom_descriptor.h"

TestSuite(_42sh, .timeout = 15);

Test(_42sh, custom_descriptor1)
{
    struct custom_FILE *f =
        createfrom_string("Salut!\nJe suis un test\nEt je te veux du mal");
    char *buffer = malloc(128 * sizeof(char));
    char *savedbuffer = buffer;
    buffer = custom_fgets(buffer, 128, f);
    cr_assert_eq(strcmp(buffer, "Salut!\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_assert_eq(strcmp(buffer, "Je suis un test\n"), 0);
    buffer = custom_fgets(buffer, 128, f);
    cr_assert_eq(strcmp(buffer, "Et je te veux du mal"), 0);
    char *newbuffer = custom_fgets(buffer, 128, f);
    cr_assert_null(newbuffer);
    free(savedbuffer);
    custom_fclose(f);
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
