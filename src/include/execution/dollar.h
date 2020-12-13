#ifndef DOLLAR_H_
#define DOLLAR_H_

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "major.h"
#include "my_err.h"
#include "my_xmalloc.h"

char *dollar_ifs(struct major *mj, int *len_var);
char *dollar_oldpwd(struct major *mj, int *len_var);
char *dollar_uid(struct major *mj, int *len_var);
char *dollar_random(struct major *mj, int *len_var);
char *dollar_dollar(struct major *mj, int *len_var);
char *dollar_sharp(struct major *mj, int *len_var);
char *dollar_interrogation(struct major *mj, int *len_var);
char *dollar_star(struct major *mj, int *len_var);
char *dollar_at(struct major *mj, int *len_var);

char *dollar_unknown(struct major *mj, char *str, int index);

#endif /* DOLLAR_H_ */