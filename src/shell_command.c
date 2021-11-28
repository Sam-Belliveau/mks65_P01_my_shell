#include "shell_command.h"

// Function that shifts over everything by 1 letter
static void remove_char(char* str)
{ while(str && (str[0] = str[1])) ++str; }

static void remove_word(char** word)
{ while(word && (word[0] = word[1])) ++word; }

static struct shell_command* shell_command_redirects(struct shell_command* command)
{
    char** j;
    int i;
    int status, fd;

    if(command)
    {
        for(i = 0; i < command->argc - 1; ++i)
        {
            status = 0;

            // Check for the different types of redirection and update status
            // If they fail, add -1 as a status.
            if(strcmp(command->argv[i], ">") == 0)
            {
                if(command->redir_stdout != SH_STDOUT) status = -1;
                else status = 1, fd = open(command->argv[i + 1], O_WRONLY | O_EXCL | O_CREAT, 0666);
            }

            else if(strcmp(command->argv[i], ">>") == 0)
            {
                if(command->redir_stdout != SH_STDOUT) status = -1;
                else status = 2, fd = open(command->argv[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0666);
            }

            else if(strcmp(command->argv[i], "<") == 0)
            {
                if(command->redir_stdin != SH_STDIN) status = -1;
                else status = 3, fd = open(command->argv[i + 1], O_RDONLY);
            }

            // Depending on the status, do different things
            switch (status)
            {
            case 0: break;
            case -1:
                fprintf(stderr, SH_PROGRAM_NAME ": warning: multiple redirects / pipes are not supported\n");
                break;
            
            default:
                if(fd < 0)
                {
                    fprintf(stderr, SH_PROGRAM_NAME ": unable to redirect %s: %s [%d]\n", command->argv[i + 1], strerror(errno), errno);
                } else
                {
                    if(status == 3) command->redir_stdin = fd;
                    else command->redir_stdout = fd;

                    command->argc -= 2;
                    free(command->argv[i]); remove_word(&command->argv[i]);
                    free(command->argv[i]); remove_word(&command->argv[i]);
                    --i;
                }
                break;
            }
        }

        return command;
    }
    
    return NULL;
}

// Remove empty commands from the chain
static struct shell_command* shell_command_compact(struct shell_command* command)
{
    if(command != NULL)
    {
        // If command has no arguments, get rid of it
        if(command->argc > 0) return shell_command_redirects(command);
        else return shell_command_compact(shell_command_free_individual(command));
    }

    else return shell_command_redirects(command);
}

// Add argument to shell command
static void shell_command_add_argument(struct shell_command* command, char* begin, char* end)
{
    int size;
    char* buf;

    // See how long buffer is
    size = end - begin;
    
    // Get rid of empty space
    if(size != 0) 
    {    
        if(command->argc < SH_MAX_ARGS)
        {
            // Copy buffer into command
            buf = calloc(size + 1, sizeof(char));
            strncpy(buf, begin, size);
            command->argv[command->argc++] = buf;
        }
        else fprintf(stderr, SH_PROGRAM_NAME ": warning: too many arguments, ignoring \"%.*s\" [MAX_ARGS=%d]\n", size, begin, SH_MAX_ARGS);
    }
}

// Initialize shell command
struct shell_command* shell_command_create(char *begin)
{
    char quote = '\0';
    char *end, *buf;
    int pipe_status, fds[2];

    struct shell_command* command = calloc(1, sizeof(struct shell_command));

    if(command == NULL)
    {
        fprintf(stderr, SH_PROGRAM_NAME ": fatal error allocating memory! exiting...\n");
        exit(-1);
    }

    command->argc = 0;
    command->next_command = NULL;

    command->redir_stdin = SH_STDIN;
    command->redir_stdout = SH_STDOUT;
    command->redir_stderr = SH_STDERR;

    // Read input until the end of the command
    for(end = begin;; ++end)
    {
        // If you are in quote mode, follow special rules
        if(quote)
        {
            // If you reach the end of the quote, 
            // then add it to the arguments
            if(*end == quote)
            {
                shell_command_add_argument(command, begin, end);
                begin = end + 1;
                quote = '\0';
            }

            // If your reach a null pointer, 
            // just add everything before it
            else if(*end == '\0')
            {
                shell_command_add_argument(command, begin, end);
                return shell_command_compact(command);
            }
        }
        
        // There is no quote, so handle it normally
        else switch(*end)
        {
            // If there is a space, just add argument to arguments
            case ' ':
                shell_command_add_argument(command, begin, end);
                begin = end + 1;
                break;

            // Delimiters and Command Ends split up commands 
            case ';': case '\n':
                shell_command_add_argument(command, begin, end);
                command->next_command = shell_command_create(end + 1);
                return shell_command_compact(command);

            case '|':
                shell_command_add_argument(command, begin, end);
                command->next_command = shell_command_create(end + 1);

                if(command->next_command != NULL)
                {
                    if(command->next_command->redir_stdin != SH_STDIN)
                    {
                        fprintf(stderr, SH_PROGRAM_NAME ": warning: multiple redirects / pipes are not supported\n");
                    }
                    else
                    {
                        pipe_status = pipe(fds);

                        if(pipe_status < 0) 
                        {
                            fprintf(stderr, SH_PROGRAM_NAME ": Error when piping %s to %s: %s [%d]\n", command->argv[0], command->next_command->argv[0], strerror(errno), errno);
                        }
                        else
                        {
                            command->redir_stdout = fds[1];
                            command->next_command->redir_stdin = fds[0];
                        }
                    }
                }
                else
                {
                    fprintf(stderr, SH_PROGRAM_NAME ": warning: unable to pipe into undefined command\n");
                }

                return shell_command_compact(command);

            // Enter special interpretation mode with quotes
            case '\'': case '\"':
                quote = *end;
                shell_command_add_argument(command, begin, end);
                begin = end + 1;
                break;

            // If redirection character is seen, add it as its own argument
            case '>':
                shell_command_add_argument(command, begin, end);
                begin = end;
                if(end[1] == '>') ++end;
                shell_command_add_argument(command, begin, end + 1);
                begin = end + 1;
                break;

            case '<':
                shell_command_add_argument(command, begin, end);
                begin = end;
                shell_command_add_argument(command, begin, end + 1);
                begin = end + 1;
                break;

            // Null characters end the command
            case '\0':
                shell_command_add_argument(command, begin, end);
                return shell_command_compact(command);

            case '\\':
                switch(end[1])
                {
                    // Escape codes built into the commands
                    case 'a': end[1] = '\a'; remove_char(end); break;
                    case 'b': end[1] = '\b'; remove_char(end); break;
                    case 'f': end[1] = '\f'; remove_char(end); break;
                    case 'n': end[1] = '\n'; remove_char(end); break;
                    case 'r': end[1] = '\r'; remove_char(end); break;
                    case 't': end[1] = '\t'; remove_char(end); break;
                    case 'v': end[1] = '\v'; remove_char(end); break;

                    // The most common use of backslash is to
                    // have the next character be ignored by 
                    // interpretation
                    default:
                        remove_char(end);
                        break;
                }
                break;

            default:
                break;
        }
    }

    return shell_command_compact(command);
}

struct shell_command* shell_command_free_individual(struct shell_command* command)
{
    int i;
    struct shell_command* next_command;

    // If the command is not NULL, free the current command
    // and then return the next command in the chain
    if(command)
    {
        next_command = command->next_command;
        
        for(i = 0; i < SH_MAX_ARGS; ++i)
            free(command->argv[i]);

        free(command);

        return next_command;
    }

    else return NULL;
}

void shell_command_free(struct shell_command* command)
{ 
    // Free all the commands until we hit the end of the chain
    while((command = shell_command_free_individual(command)));
}