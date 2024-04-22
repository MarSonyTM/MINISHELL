/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:31:00 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/22 14:09:31 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal_caught = 0;

void	handle_sigint(int sig)
{
    (void)sig;
    g_signal_caught = 1;
    char *line = rl_copy_text(0, rl_end);  // Copy the current line
    if (line && *line) {
        add_history(line);  // Add the line to the history
    }
    free(line);  // Free the copied line
    write(STDOUT_FILENO, "\n", 1);  // Display a new prompt on a new line
    rl_on_new_line();  // Move to a new line
    rl_replace_line("", 0);  // Clear the current input line
    rl_redisplay();  // Redraw the current input line
}

void handle_sigquit(int sig) {
    (void)sig;
    
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
