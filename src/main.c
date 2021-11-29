#include "shell.h"
#include "shell_command.h"

#include <stdio.h>
#include <sys/stat.h>

// Use to check if PID value is a child or not in signal_handler
static pid_t parent_pid;

// Handle SIGINT so that the shell can survive a ctrl+c
static void signal_handler(int);

// Execute a file like a script
static void execute_file(char* file);

int main(int argc, char** argv)
{
    int i;
    struct shell_command* command;
    parent_pid = getpid();

    // If there are no arguments, run the shell prompt
    if(argc == 1)
    {
        printf("\n\t " 
            SH_COLOR_MAGENTA SH_PROGRAM_NAME SH_COLOR_RESET 
            " ("SH_COLOR_CYAN"S"SH_COLOR_RESET"am "
                SH_COLOR_CYAN"A"SH_COLOR_RESET"nd "
                SH_COLOR_CYAN"L"SH_COLOR_RESET"ogan's "
                SH_COLOR_CYAN"SH"SH_COLOR_RESET"ell) " 
                SH_COLOR_GREEN SH_VERSION_NO SH_COLOR_RESET 
        "!\n");

        signal(SIGINT, signal_handler);

        // Very Simple Shell Loop
        while(1)
        {
            shell_print_header();

            command = shell_get_user_line();
            shell_execute_commands(command);
            shell_command_free(command);
        }
    }

    // Treat the arguments to the shell like input files
    else for(i = 1; i < argc; ++i)
        execute_file(argv[i]);
}

// Handle SIGINT by not closing if it is the parent process
// and exiting if it is the child. The child usually overwrites this however.
static void signal_handler(int signal) {
    if(signal == SIGINT)
    {
        if(getpid() == parent_pid) 
            fprintf(stderr, "(SIGINT)");
        else 
            exit(-1);
    }
}

/**
 * @brief execute a file as if it were the input to the shell
 * 
 * @param file file to execute
 */
static void execute_file(char* file)
{
    int len;
    int fd;
    char* buf = NULL;
    struct shell_command* command;

    struct stat file_stats;
    stat(file, &file_stats);
    len = file_stats.st_size;

    buf = calloc(len + 1, sizeof(char));

    if(!buf) 
    {
        fprintf(stderr, SH_PROGRAM_NAME ": error: unable to allocate memory when reading %s\n", file);
        return;
    }

    fd = open(file, O_RDONLY);

    if(fd < 0)
    {
        fprintf(stderr, SH_PROGRAM_NAME ": error: unable to open %s: %s [%d]\n", file, strerror(errno), errno);
        return;
    }
    
    read(fd, buf, len);

    command = shell_command_create(buf);
    shell_execute_commands(command);
    shell_command_free(command);

    close(fd);
}