# SALSH

**S***am* **A***nd* **L***ogan's* **SH***ell*s

---

## Testing

There are two ways to test this shell.

`make run`: will give you a prompt to type tests into

`make test`: will run test script you can find in `./scripts/test_script.salsh`


---

## Features

1. Commands with up to 128 arguments

2. Lenient formatting allowing you to put spaces or redirection's anywhere you like

3. Multiple commands separated with `\n` or `;`

4. Grouping blocks of texts into single arguments using `'` or `"`

5. Using `\` to escape characters from being considered in formatting

6. Using `>` and `>>` to redirect stdout

7. Using `<` to redirect to stdin

8. Using `|` to pipe stdout of a command to the stdin of the next command

9. Very thorough error reporting / crash protection

10. Support for `~` when using `cd` or when reporting current directory

11. Support for `quit` and `exit`

12. Support for running scripts when given as arguments to the shell

13. Interactive features such as (GNU Readline):

    1.  Command History using arrow keys

    2.  Tab Auto completion

    3.  Moving cursor back and forth

---

### Links

HW Assignment: <https://www.stuycs.org/systems-dw/2021/11/17/p01.html>

---

### Header Files

**script.h**
```c
// Execute a file like a script
void execute_file(char* file);
```


**shell_command.h**
```c
struct shell_command 
{
    int argc;
    char* argv[SH_MAX_ARGS + 1];

    int redir_stdin;
    int redir_stdout;
    int redir_stderr;

    struct shell_command* next_command;
};

// Initialize shell command
struct shell_command* shell_command_create(char *);

// Scan arguments for redirections, and add them to the command
struct shell_command* shell_command_add_redirects(struct shell_command*);

// Free command and return the next command in the chain
struct shell_command* shell_command_free_individual(struct shell_command*);

// Free the entire chain of commands.
void shell_command_free(struct shell_command*);
```


**shell.h**
```c
// Print prompt and reads user input
char* shell_readline();

// Execute every command in the chain
void shell_execute_commands(struct shell_command*);

// Execute a single command and handle file descriptors / forking
void shell_execute(struct shell_command*);
```

**constants.h**
```c
#define SH_PROGRAM_NAME "SALSH"

#define SH_VERSION_NO "v2.0.1"

#define SH_MAX_ARGS (1 << 8)

#define SH_CWD_SIZE (1 << 12)
#define SH_USR_SIZE (1 << 10)
#define SH_HEADER_SIZE (1 << 13)

#define SH_USER_INPUT_BUFFER (1 << 12)

#define SH_STDIN STDIN_FILENO
#define SH_STDOUT STDOUT_FILENO
#define SH_STDERR STDERR_FILENO

#define SH_TRUE 1
#define SH_FALSE 0

#define SH_COLOR_BLACK "\x001b[30m"
#define SH_COLOR_RED "\x001b[31m"
#define SH_COLOR_GREEN "\x001b[32m"
#define SH_COLOR_YELLOW "\x001b[33m"
#define SH_COLOR_BLUE "\x001b[34m"
#define SH_COLOR_MAGENTA "\x001b[35m"
#define SH_COLOR_CYAN "\x001b[36m"
#define SH_COLOR_WHITE "\x001b[37m"
#define SH_COLOR_RESET "\x001b[0m"

// Close a file descriptor and set it to the default,
// but only close the file descriptor if it does not equal default
#define safe_close(fd, tfd) do { if((fd) != (tfd)) { close(fd); fd = tfd; }} while(0)
```