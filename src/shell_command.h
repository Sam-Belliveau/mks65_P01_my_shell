#ifndef SHELL_COMMAND_HEADER_FILE
#define SHELL_COMMAND_HEADER_FILE 1

#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>

#include <errno.h>

#include "constants.h"

struct shell_command 
{
    int argc;
    char* argv[SH_MAX_ARGS];
    struct shell_command* next_command;

    int redir_stdin;
    int redir_stdout;
    int redir_stderr;
};

struct shell_command* shell_command_create(char *);

struct shell_command* shell_command_free_individual(struct shell_command*);
void shell_command_free(struct shell_command*);

#endif