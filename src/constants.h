#ifndef SHELL_CONSTANTS_HEADER_FILE
#define SHELL_CONSTANTS_HEADER_FILE

#include <stdio.h>

#define SH_MAX_ARGS 64

#define SH_SPACE ' '
#define SH_DELIMITER ';'
#define SH_COMMAND_END '\n'

#define tprintf(n, args...) do{for(int TPFL=(n);TPFL>1;--TPFL){printf("│   ");}if(n){printf("├───");}printf(args);}while(0)

#endif