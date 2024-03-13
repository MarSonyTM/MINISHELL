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
