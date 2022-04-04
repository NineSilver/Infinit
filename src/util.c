#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>

#include "sig.h"
#include "util.h"

// Colors uwu
static const char* loglevel_to_text[4] = {
    [LOG_INFO] = "[\e[1;34mINFO\e[0m] ",
    [LOG_WARN] = "[\e[1;33mWARN\e[0m] ",
    [LOG_ERROR] = "[\e[1;31mERROR\e[0m] ",
    [LOG_NOPREFIX] = ""
};

// Bind the console to stout, stdin and stderr. Only available after mounting /dev
void console_init(void)
{
    int conout = open("/dev/console", O_RDWR);
    int conin = open("/dev/console", O_RDONLY);

    dup2(conin, 0); // stdin
    dup2(conout, 1); // stdout
    dup2(conout, 2); // stderr

    if(conin > 2) close(conin);
    if(conout > 2) close(conout);
}

// perror() sucks, so we have our own wrapper
void infinit_log(enum loglevel level, const char* msg, ...)
{
    printf("%sInfinit: ", loglevel_to_text[level]);

    va_list ap;
    va_start(ap, msg);

    vprintf(msg, ap);

    va_end(ap);

    putchar('\n');
}

int spawn(char* argv[])
{
    pid_t pid = fork();

    if(pid == 0)
    {
        setsid();
        sig_defaults();

        execvp(argv[0], argv);
        exit(EX_OSERR);
    }
    else
    {
        return -1;
    }

    int status;
    if(waitpid(pid, &status, 0) == -1)
        return 1;
    
    return WEXITSTATUS(status);
}
