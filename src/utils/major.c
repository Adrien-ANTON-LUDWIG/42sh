#include "custom_descriptor.h"
#include "my_utils.h"
#include "my_xmalloc.h"
/**
 * @brief Initializes major
 *
 * @return struct major*
 */
struct major *major_init(void)
{
    return my_xcalloc(NULL, 1, sizeof(struct major));
}

/**
 * @brief Frees struct major
 *
 * @param mj
 */
void major_free(struct major *mj)
{
    custom_fclose(mj->file);
    list_free(mj->variables);
    free(mj);
}