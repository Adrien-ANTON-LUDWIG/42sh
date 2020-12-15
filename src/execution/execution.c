#include "execution.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "custom_descriptor.h"

int run_command(struct major *mj, char **args)
{
    fflush(stdout);
    int pid = fork();
    if (!pid)
    {
        custom_fclose(mj->file);
        execvp(args[0], args);
        exit(127);
    }
    int rvalue = 0;
    waitpid(pid, &rvalue, 0);
    rvalue = WEXITSTATUS(rvalue);
    mj->rvalue = rvalue;
    if (rvalue == 127)
        fprintf(stderr, "Command not found : %s\n", args[0]);
    return rvalue;
}