#ifndef SHELL_HEADER_FILE
#define SHELL_HEADER_FILE 1

#include <unistd.h>
#include <sys/wait.h>

#include <stdlib.h>

#include "constants.h"
#include "shell_command.h"

void shell_print_header();

struct shell_command* shell_get_user_line();

void shell_execute_commands(struct shell_command*);

void shell_execute(struct shell_command*);
#endif