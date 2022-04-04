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

void sig_defaults(void)
{
    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGPWR);
    sigaddset(&set, SIGSTOP);
    sigaddset(&set, SIGTSTP);
    sigaddset(&set, SIGCONT);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    struct sigaction sa;
    for(int i = 1; i < NSIG; i++)
    {
        sa.sa_handler = SIG_DFL;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(i, &sa, NULL);
    }
}
