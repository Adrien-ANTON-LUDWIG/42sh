#include "my_err.h"

#include <err.h>

void my_err(int err_code, struct major *mj, char *message)
{
    major_free(mj);
    errx(err_code, "%s", message);
}

int my_soft_err(struct major *mj, int err_code, char *message)
{
    mj->rvalue = err_code;
    warnx("%s", message);
    return err_code;
}