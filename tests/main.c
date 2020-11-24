#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>


TestSuite(minimake, .timeout = 15);

Test(minimake, is_command_valid)
{

}

Test(minimake, is_command_invalid)
{

}

Test(minimake, expand_command)
{

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
