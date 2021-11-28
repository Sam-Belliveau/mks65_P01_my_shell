#include "shell.h"
#include "shell_command.h"

#include <stdio.h>

int main(void)
{
    struct shell_command* user_command;

    printf("Welcome to " SH_PROGRAM_NAME " " SH_VERSION_NO "!\n");

    while(1)
    {
        shell_print_header();

        user_command = shell_get_user_line();
        shell_execute_commands(user_command);
        shell_command_free(user_command);
    }
}