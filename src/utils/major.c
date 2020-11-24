#include "my_utils.h"

void major_init(void)
{}

void major_free(struct major *mj)
{
    free(mj);
}