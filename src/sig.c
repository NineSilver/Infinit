#include <signal.h>
#include <stdlib.h>

#include "sig.h"

static void sigchld_hand(int sig, siginfo_t* info, void* data)
{
    (void)sig;
    (void)info;
    (void)data;
}

void sig_init(void)
{
    struct sigaction sa;
    for(int i = 1; i < NSIG; i++)
    {
        sa.sa_handler = SIG_IGN;
        sa.sa_flags = SA_RESTART;
        sigemptyset(&sa.sa_mask);
        sigaction(i, &sa, NULL);
    }

    sa.sa_sigaction = sigchld_hand;
    sa.sa_flags = SA_SIGINFO | SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);
}
