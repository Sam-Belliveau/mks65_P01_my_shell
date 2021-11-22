#include "shell.h"

void shell_print_header()
{
    char cwd[SH_CWD_SIZE] = {};
    char usr[SH_USR_SIZE] = {};

    getcwd(cwd, SH_CWD_SIZE);
    gethostname(usr, SH_USR_SIZE);

    printf(SH_COLOR_RESET "\n");
    printf(SH_COLOR_RESET "─────╮ " SH_COLOR_RED SH_PROGRAM_NAME SH_COLOR_RESET " : " SH_COLOR_GREEN "%s\n", usr);
    printf(SH_COLOR_RESET " ╭───╯ " SH_COLOR_BLUE "%s\n", cwd);
    printf(SH_COLOR_RESET "─╯ ");
}

struct shell_command* shell_get_user_line()
{
    char buf[SH_USER_INPUT_BUFFER + 1] = {};

    fgets(buf, SH_USER_INPUT_BUFFER + 1, stdin);

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
    int t_stdin, t_stdout, t_stderr;
    int status, f;

    // Throw out empty commands
    if(command == NULL) return;
    if(command->argc == 0) return;

    // Handle CD
    if(strcmp(command->argv[0], "cd") == 0)
    {
        if(command->argc != 2)
        {
            printf(SH_PROGRAM_NAME ": cd: 1 argument required, %d given\n", command->argc - 1);
        } else
        {
            status = chdir(command->argv[1]);

            // print out error if cd fails
            if(status) printf(SH_PROGRAM_NAME ": cd: %s [%d]\n", strerror(errno), errno);
        }
    }

    // Handle quit
    else if(
        strcmp(command->argv[0], "quit") == 0 ||
        strcmp(command->argv[0], "exit") == 0
    ) {
        exit(0);
    }

    // If no special command is entered
    // fork and run process
    else
    {
        // Make copies of standard fds
        t_stdin  = dup(SH_STDIN);
        t_stdout = dup(SH_STDOUT);
        t_stderr = dup(SH_STDERR);

        // Pipe outputs to the commands specified outputs
        dup2(command->redir_stdin,  SH_STDIN);
        dup2(command->redir_stdout, SH_STDOUT);
        dup2(command->redir_stderr, SH_STDERR);

        // Fork Process
        f = fork();
            
        // Child
        if(f == 0) 
        {
            status = execvp(command->argv[0], command->argv);
            
            // If there is an error, return it
            if(status) exit(errno);
            else exit(0);
        }

        // Have parent wait for child
        else 
        {
            waitpid(f, &status, 0);
            status = WEXITSTATUS(status);

            // Handle different return values from child
            switch(status)
            {
                // Returned Correctly, no error
                case 0: break;

                // "No such file or directory" = Command doesn't exist
                case 2:
                    printf(SH_PROGRAM_NAME ": command not found: %s\n", command->argv[0]);
                    break;
                
                // Handle every other error
                default:
                    printf(SH_PROGRAM_NAME ": %s [%d]\n", strerror(status), status);
                    break;
            }
        }

        // Close the files opened by the command
        close(SH_STDIN);
        close(SH_STDOUT);
        close(SH_STDERR);

        // Move the duplicated standard pipes back to the original spots
        dup2(t_stdin,  SH_STDIN);
        dup2(t_stdout, SH_STDOUT);
        dup2(t_stderr, SH_STDERR);

        // Close the duplicates
        close(t_stdin);
        close(t_stdout);
        close(t_stderr);
    }
}