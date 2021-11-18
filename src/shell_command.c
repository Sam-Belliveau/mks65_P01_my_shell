#include "shell_command.h"

struct shell_command* shell_command_create(char *user_line)
{
    int size;
    char *i, *buf;
    struct shell_command* command = calloc(1, sizeof(struct shell_command));
    
    command->argc = 0;
    command->next_command = NULL;

    for(i = user_line; *user_line; ++i)
    {
        if(*i == SH_SPACE 
        || *i == SH_DELIMITER 
        || *i == SH_COMMAND_END 
        || *i == '\0')
        {
            // See how long buffer is
            size = i - user_line;
            
            // Get rid of empty space
            if(size != 0) 
            {    
                // Copy buffer into command
                buf = calloc(size + 1, sizeof(char));
                strncpy(buf, user_line, size);
                command->argv[command->argc++] = buf;
            }

            // Based on type of delimiter, handle it differently
            if(*i == SH_DELIMITER)
            {
                command->next_command = shell_command_create(i + 1);
                break;
            }

            else if(*i == SH_COMMAND_END || *i == '\0')
            {
                command->next_command = NULL;
                break;
            }

            else user_line = i + 1;
        }
    }

    return command;
}

void shell_command_print(int l, struct shell_command* command)
{
    int i;
    tprintf(l, "Command (%d args): ", command->argc);
    for(i = 0; i < command->argc; ++i) printf("\"%s\" ", command->argv[i]);
    printf("\n");

    if(command->next_command) shell_command_print(l + 1, command->next_command);
}

void shell_command_free(struct shell_command* command)
{
    int i;

    if (command)
    {
        shell_command_free(command->next_command);

        for(i = 0; i < SH_MAX_ARGS; ++i)
            free(command->argv[i]);

        free(command);
    }
}