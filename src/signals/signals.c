#include "../../inc/minishell.h"

void handle_sigint(int sig) 
{
    printf("\nCaught signal %d\n", sig);
    printf("minishell> ");
}

void handle_sigquit(int sig) 
{
    printf("\nCaught signal %d\n", sig);
}

void check_blocked_signals()
 {
    sigset_t blocked;
    sigemptyset(&blocked);

    if (sigprocmask(SIG_BLOCK, NULL, &blocked) < 0) {
        perror("sigprocmask");
        return;
    }

    // if (sigismember(&blocked, SIGINT)) {
         
    // } else {
    //     printf("SIGINT is not blocked\n");
    // }

    // if (sigismember(&blocked, SIGQUIT)) {
    //     printf("SIGQUIT is blocked\n");
    // } else {
    //     printf("SIGQUIT is not blocked\n");
    // }
}
