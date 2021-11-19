#include "shell_command.h"

// Function that shifts over everything by 1 letter
static void remove_char(char* str)
{ while((str[0] = str[1])) ++str; }

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
        // Copy buffer into command
        buf = calloc(size + 1, sizeof(char));
        strncpy(buf, begin, size);
        command->argv[command->argc++] = buf;
    }
}

// Initialize shell command
struct shell_command* shell_command_create(char *begin)
{
    char quote = '\0';
    char *end, *buf;
    struct shell_command* command = calloc(1, sizeof(struct shell_command));

    command->argc = 0;
    command->next_command = NULL;

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
                return command;
            }
        }
        
        // There is no quote, so handle it normally
        else switch(*end)
        {
            // If there is a space, just add argument to arguments
            case SH_SPACE:
                shell_command_add_argument(command, begin, end);
                begin = end + 1;
                break;

            // Delimiters and Command Ends split up commands 
            case SH_DELIMITER:
            case SH_COMMAND_END:
                shell_command_add_argument(command, begin, end);
                command->next_command = shell_command_create(end + 1);
                return command;

            // Enter special interpretation mode with quotes
            case SH_QUOTE_SINGLE:
            case SH_QUOTE_DOUBLE:
                quote = *end;
                shell_command_add_argument(command, begin, end);
                begin = end + 1;
                break;

            // Null characters end the command
            case '\0':
                shell_command_add_argument(command, begin, end);
                return command;

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

    return command;
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