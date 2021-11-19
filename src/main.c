#include "shell.h"
#include "shell_command.h"

#include <stdio.h>

int main(void)
{
    printf("Welcome to " SH_PROGRAM_NAME " " SH_VERSION_NO "!\n");
    
    while(1)
    {
        shell_print_header();
        shell_execute_commands(shell_get_user_line());
    }
}