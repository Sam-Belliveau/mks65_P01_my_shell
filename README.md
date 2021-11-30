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

---

### Links

HW Assignment: <https://www.stuycs.org/systems-dw/2021/11/17/p01.html>
