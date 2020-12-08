#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H

#include "custom_descriptor.h"

#define IS_NEWLINE "\n\r"
#define MY_IS_SPACE " \t\v"
#define IS_NOT_WORD " \t\v\n\r;&|<>()#"
#define IS_OPERATOR "&|()<>;\n"
#define IS_REDIR "<>&|"

int is_in(char c, char *s);
int is_not_in(char c, char *s);
int at_end(struct custom_FILE *f);
char get_char(struct custom_FILE *f, int incr);
void skip(struct custom_FILE *f, int (*predicate)(char, char *), char *s);

#endif