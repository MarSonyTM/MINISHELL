/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:31:00 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/22 12:36:43 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal_caught = 0;

void handle_sigint(int sig)
{
    (void)sig;
    g_signal_caught = 1;
    write(STDOUT_FILENO, "\n", 1);  // Display a new prompt on a new line
    rl_on_new_line();  // Move to a new line
    rl_replace_line("", 0);  // Clear the current input line
    rl_redisplay();  // Redraw the current input line
}

void handle_sigquit(int sig) {
    (void)sig;
    // Do nothing
}

void handle_eof() 
{
    exit(0);  // Exit the shell
}

void setup_signals()
 {
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit); 
}

void	check_blocked_signals(void)
{
	sigset_t	blocked;

	sigemptyset(&blocked);
	if (sigprocmask(SIG_BLOCK, NULL, &blocked) < 0)
	{
		perror("sigprocmask");
		return ;
	}
}
