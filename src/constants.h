#ifndef SHELL_CONSTANTS_HEADER_FILE
#define SHELL_CONSTANTS_HEADER_FILE

#include <stdio.h>

#define SH_PROGRAM_NAME "SALSH"

#define SH_VERSION_NO "v2.0.0"

#define SH_MAX_ARGS 128

#define SH_CWD_SIZE 4096
#define SH_USR_SIZE 1024

#define SH_USER_INPUT_BUFFER 2048

#define SH_STDIN STDIN_FILENO
#define SH_STDOUT STDOUT_FILENO
#define SH_STDERR STDERR_FILENO

#define SH_TRUE 1
#define SH_FALSE 0

#define SH_COLOR_BLACK "\x001b[30m"
#define SH_COLOR_RED "\x001b[31m"
#define SH_COLOR_GREEN "\x001b[32m"
#define SH_COLOR_YELLOW "\x001b[33m"
#define SH_COLOR_BLUE "\x001b[34m"
#define SH_COLOR_MAGENTA "\x001b[35m"
#define SH_COLOR_CYAN "\x001b[36m"
#define SH_COLOR_WHITE "\x001b[37m"
#define SH_COLOR_RESET "\x001b[0m"

#endif