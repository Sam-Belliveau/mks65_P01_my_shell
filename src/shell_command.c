#include "shell_command.h"

struct shell_command* shell_command_create(char *user_line)
{
    int size, j;
    char* i;
    char* buf;
    struct shell_command* command = malloc(sizeof(struct shell_command));
    
    command->argc = 0;
    command->next_command = NULL;

    for(j = 0; j < SH_MAX_ARGS; ++j) command->argv[j] = NULL;

    while(*user_line)
    {
        for(i = user_line;; ++i)
        {
            if(*i == SH_SPACE || *i == SH_DELIMITER || *i == SH_COMMAND_END)
            {
                size = i - user_line;
                buf = calloc(size + 1, sizeof(char));
                strlcpy(buf, user_line, size);
                command->argv[command->argc++] = buf;
                
                if(*i == SH_DELIMITER)
                {
                    command->next_command = shell_command_create(i + 1);
                    return command;
                }

                else if(*i == SH_COMMAND_END)
                {
                    command->next_command = NULL;
                    return command;
                }

                else break;
            }

        }

        user_line = i;
    }
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