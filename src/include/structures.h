#ifndef STRUCTURES_H
#define STRUCTURES_H

struct major
{
    int rvalue;
    int options;
    struct custom_FILE *file;
    struct list *variables;
    struct token *pending;
};

#endif /* STRUCTURES_H */