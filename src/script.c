#include "script.h"

/**
 * @brief execute a file as if it were the input to the shell
 * 
 * @param file file to execute
 */
void execute_file(char* file)
{
    int fd;
    char* buf = NULL;
    struct shell_command* command;

    // get the length of the file and allocate a buffer
    struct stat file_stats;
    stat(file, &file_stats);
    buf = calloc(file_stats.st_size + 1, sizeof(char));

    if(!buf) 
    {
        fprintf(stderr, SH_PROGRAM_NAME ": error: unable to allocate memory when reading %s\n", file);
        return;
    }

    // open the file that you want to read
    fd = open(file, O_RDONLY);

    if(fd < 0)
    {
        fprintf(stderr, SH_PROGRAM_NAME ": error: unable to open %s: %s [%d]\n", file, strerror(errno), errno);
        return;
    }
    
    // read in the script into thr buffer
    if(read(fd, buf, file_stats.st_size) != file_stats.st_size)
    {
        fprintf(stderr, SH_PROGRAM_NAME ": warning: detected issues when reading from file %s\n", file);
    }

    // create the command from the buffer and execute it
    command = shell_command_create(buf);
    shell_execute_commands(command);
    shell_command_free(command);

    // free the buffer and close the file
    free(buf);
    close(fd);
}