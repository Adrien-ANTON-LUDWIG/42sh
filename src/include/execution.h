#ifndef EXECUTION_H
#define EXECUTION_H

#include "tokens.h"

int run_command(char **args);
char **token_list_to_char_array(struct list *l);

#endif