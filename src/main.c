#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fs.h"
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

    for(;;);
    return 0;
}
