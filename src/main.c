#include <errno.h>
#include <paths.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fs.h"
#include "sig.h"
#include "util.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv; // Until we do this TODO below, we will make gcc doesn't complain
    
    if(getpid() != 1) // Not running as pid 1; TODO: initctl or sth
        return 1;
    
    fs_mount_early();
    console_init();

    infinit_log(LOG_NOPREFIX, "Hello, %s!", "World");
    if(chdir("/") != 0)
    {
        panic("Impossible to change cwd to '/': %s", strerror(errno));
    }

    setenv("USER", "root", 1);
    setenv("HOME", "/root", 1);
    setenv("PATH", _PATH_STDPATH, 1);
    setenv("SHELL", _PATH_BSHELL, 1);

    sig_init();

    fs_mount_everything();

    for(;;);
    return 0;
}
