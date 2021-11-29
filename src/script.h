#ifndef SHELL_SCRIPT_HEADER_FILE
#define SHELL_SCRIPT_HEADER_FILE

#include "constants.h"
#include "shell.h"
#include "shell_command.h"

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/stat.h>

// Execute a file like a script
void execute_file(char* file);

#endif