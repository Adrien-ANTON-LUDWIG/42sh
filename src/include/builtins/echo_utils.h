#ifndef _ECHO_UTILS_H
#define _ECHO_UTILS_H

int set_options(char *argv[], int *n, int *e, int *E);
int nb_to_read_oct(char *s, int i);
int nb_to_read_hx(char *s, int i);
int str_oct_to_dec(char *s, int i, int to_read);
int str_hx_to_dec(char *s, int i, int to_read);

#endif /* _ECHO_UTILS_H */