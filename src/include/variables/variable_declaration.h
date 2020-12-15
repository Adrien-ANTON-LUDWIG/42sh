#ifndef VARIABLE_DECLARATION_H
#define VARIABLE_DECLARATION_H

void variable_declare(struct major *mj, char *name, char *value);
char **get_assignment_data(struct major *mj, struct token *tk);

#endif