#include "shell.h"

void shell_print_header()
{
    char cwd[SH_CWD_SIZE];

    getcwd(cwd, SH_CWD_SIZE);

    printf(SH_PROGRAM_NAME ":%s> ", cwd);
}

struct shell_command* shell_get_user_line()
{
    char buf[1000000];

    gets(buf);

    return shell_command_create(buf);
}

void shell_execute_commands(struct shell_command* command)
{
    if(command != NULL && command->argc > 0)
    {
        shell_execute(command);
        shell_execute_commands(command->next_command);
    }
}

void shell_execute(struct shell_command* command)
{
    int status, f;
    if(strcmp(command->argv[0], "cd") == 0)
    {
        if(command->argc != 2)
        {
            printf("error: cd: 1 argument required, %d given\n", command->argc - 1);
        } else
        {
            chdir(command->argv[1]);
        }
    }

    else if(strcmp(command->argv[0], "quit") == 0)
    {
        printf("Goodbye!\n");
        exit(0);
    }

    // If no special command is entered
    // fork and run process
    else
    {
        f = fork();
            
        // Child
        if(!f) 
        {
            execvp(command->argv[0], command->argv);
            exit(0); // TODO: is this right?
        }

        // Have parent wait for child
        else waitpid(f, &status, 0);
    }
}