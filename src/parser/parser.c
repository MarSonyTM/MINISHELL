/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:08 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/09 12:56:20 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_tokens(t_token *tokens, t_cmd **cmd, t_env *env)
{
	t_command	command;
	t_cmd		*current_cmd;
	int			arg_count;
	t_token		*current;
	int			result;

	current_cmd = NULL;
	arg_count = 0;
	current = tokens;
	command.current = &current;
	command.current_cmd = &current_cmd;
	command.cmd = &cmd;
	command.env = &env;
	command.arg_count = &arg_count;
	while (current != NULL)
	{
		result = process_token(&command);
		if (result != 0)
			return (result);
		if (!current_cmd)
			return (1);
		current = current->next;
	}
	return (0);
}

// Main parse function
int	parse(t_token *tokens, t_cmd **cmd, t_env *env)
{
	if (!tokens)
		return (2);
	return (process_tokens(tokens, cmd, env));
	return (0);
}
