/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:31:00 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/11 15:55:51 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t signal_caught = 0;

void handle_sigint(int sig)
{
    (void)sig;  // Indicate that sig is intentionally unused
    signal_caught = 1;
    printf("\nminishell> ");
}

void    handle_sigquit(int sig) 
{
    (void)sig;
}

void check_blocked_signals()
 {
    sigset_t blocked;
    sigemptyset(&blocked);

    if (sigprocmask(SIG_BLOCK, NULL, &blocked) < 0)
    {
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
