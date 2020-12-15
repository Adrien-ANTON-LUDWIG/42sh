#ifndef VARIABLES_H
#define VARIABLES_H

char **replace_variables(struct major *mj, char **command);
void variable_list_free(struct major *mj);
char *get_variable_value(struct major *mj, char *name);
char *get_variable_name(struct major *mj, char *var);

#endif