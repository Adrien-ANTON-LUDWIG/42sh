#include <err.h>

#include "my_utils.h"

/**
 * @brief Frees major and crashes
 *
 * @param err_code
 * @param mj
 * @param message
 */
void my_err(int err_code, struct major *mj, char *message)
{
    major_free(mj);
    errx(err_code, "%s", message);
}
