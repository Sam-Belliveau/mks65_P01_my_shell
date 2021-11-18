#include "shell.h"
#include "shell_command.h"

#include <stdio.h>

int main(void)
{
    struct shell_command* command = shell_command_create("ls ./this_folder/;echo Hello World");
    shell_command_print(0, command);
}