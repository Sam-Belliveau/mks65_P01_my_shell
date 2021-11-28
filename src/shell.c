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

    fgets(buf, SH_USER_INPUT_BUFFER, stdin);

    return shell_command_create(buf);
}

void shell_execute_commands(struct shell_command* command)
{
    if(command != NULL)
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
            fprintf(stderr, SH_PROGRAM_NAME ": cd: 1 argument required, %d given\n", command->argc - 1);
        } else
        {
            status = chdir(command->argv[1]);

            // print out error if cd fails
            if(status) fprintf(stderr, SH_PROGRAM_NAME ": cd: %s [%d]\n", strerror(errno), errno);
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
            
            // Handle different return values from child
            switch(errno)
            {
                // Returned Correctly, no error
                case 0: break;

                // "No such file or directory" = Command doesn't exist
                case 2:
                    fprintf(stderr, SH_PROGRAM_NAME ": command not found: %s\n", command->argv[0]);
                    break;
                
                // Handle every other error
                default:
                    fprintf(stderr, SH_PROGRAM_NAME ": %s [%d]\n", strerror(errno), errno);
                    break;
            }

            exit(status);
        }

        // Have parent wait for child
        else 
        {
            // Do not wait for child if you do not need to
            if(command->redir_stdout == SH_STDOUT)
            {
                waitpid(f, &status, 0);
                status = WEXITSTATUS(status);
            }
        }

        // Close all of the outputs opened by the command
        close(SH_STDIN); close(command->redir_stdin);
        close(SH_STDOUT); close(command->redir_stdout);
        close(SH_STDERR); close(command->redir_stderr);
    
        // Move the outputs back to their place
        dup2(t_stdin,  SH_STDIN);  close(t_stdin);
        dup2(t_stdout, SH_STDOUT); close(t_stdout);
        dup2(t_stderr, SH_STDERR); close(t_stderr);
    }
}