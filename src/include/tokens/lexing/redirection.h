#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "my_utils.h"

struct redir *init_redirection(struct major *mj);
void set_redirection(struct major *mj, struct redir *redirection, char *word,
                     char *file);
void free_redirection(struct redir *redir);

#endif /* REDIRECTION_H */