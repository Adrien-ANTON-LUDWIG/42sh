#include "major.h"

#include <stdlib.h>

#include "custom_descriptor.h"
#include "list.h"
#include "my_xmalloc.h"

struct major *major_init(void)
{
    return my_xcalloc(NULL, 1, sizeof(struct major));
}

void major_free(struct major *mj)
{
    custom_fclose(mj->file);
    list_free(mj->variables);
    free(mj);
}