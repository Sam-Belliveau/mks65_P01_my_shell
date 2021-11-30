#ifndef SHELL_HEADER_FILE
#define SHELL_HEADER_FILE 1

#include <time.h>

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

#include <stdlib.h>

#include <errno.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "constants.h"
#include "shell_command.h"

// Print prompt for users to look at when typing commands
const char* shell_print_header();

// Execute every command in the chain
void shell_execute_commands(struct shell_command*);

// Execute a single command and handle file descriptors / forking
void shell_execute(struct shell_command*);

#endif