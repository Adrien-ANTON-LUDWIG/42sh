#define _GNU_SOURCE
#include "core/custom_descriptor.h"
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>


TestSuite(42sh, .timeout = 15);

Test(42sh, custom_descriptor)
{
    custom
}

Test(42sh, custom_descriptor)
{

}

Test(42sh, custom_descriptor)
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
