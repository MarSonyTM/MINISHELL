/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:31:00 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/17 13:20:57 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

sig_atomic_t	g_signal_caught = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_caught = 1;
}

void	handle_sigquit(int sig)
{
	(void)sig;
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
