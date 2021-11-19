#ifndef SHELL_CONSTANTS_HEADER_FILE
#define SHELL_CONSTANTS_HEADER_FILE

#include <stdio.h>

#define SH_PROGRAM_NAME "BashSL"

#define SH_VERSION_NO "v1.0.0"

#define SH_MAX_ARGS 64

#define SH_CWD_SIZE 256
#define SH_USR_SIZE 256

#define SH_USER_INPUT_BUFFER 2048

#define SH_SPACE ' '
#define SH_DELIMITER ';'
#define SH_COMMAND_END '\n'

#define SH_QUOTE_SINGLE '\''
#define SH_QUOTE_DOUBLE '"'

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