#ifndef SHOPT_H_
#define SHOPT_H_

#include "major.h"

int shopt_options_argv(struct major *mj, char **argv);
void init_shopt_opt_list(struct major *mj);
void free_shopt_opt_list(struct major *mj);
void set_shopt_opt(struct shopt_opt_list *sol, char *opt_name, int value);

#endif /* SHOPT_H_ */