#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(getpid() != 1)
        return 1;

    for(;;);
    return 0;
}
