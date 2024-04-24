/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:31:00 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/24 11:08:47 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal_caught = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_caught = 1;
	if (rl_end > 0)
	{
		add_history(rl_line_buffer);
	}
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	handle_eof(void)
{
	exit(0);
}

void	setup_signals(void)
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
