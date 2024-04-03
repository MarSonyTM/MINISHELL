/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:05:23 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 11:23:41 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_builtin_or_command(t_cmd **cmd, t_token *current,
	t_env *env, t_cmd **current_cmd, int *arg_count)
{
	*current_cmd = initialize_new_command(cmd, current, env);
	if (!(*current_cmd))
		return (1);
	*arg_count = 1;
	return (0);
}

int	handle_argument(t_cmd *current_cmd, t_token *current)
{
	if (current_cmd == NULL)
		return (0);
	if (add_argument_to_command(current_cmd, current->value) != 0)
		return (1);
	return (0);
}

int	handle_input(t_cmd *current_cmd, t_token *current)
{
	if (current_cmd == NULL)
		return (0);
	current_cmd->input = ft_strdup(current->value);
	if (!current_cmd->input)
		return (1);
	return (0);
}

int	handle_parser_redirection(t_cmd *current_cmd, t_token **current)
{
	int		result;

	if (current_cmd == NULL)
		return (0);
	result = handle_redirection(current_cmd, current, (*current)->type);
	if (result != 0)
		return (result);
	return (0);
}

int	handle_parser_heredoc(t_cmd **current_cmd, t_token **current)
{
	char	*heredoc_input;

	if (!(*current)->next)
	{
		error(ERR_PARS, "\n");
		return (2);
	}
	*current = (*current)->next;
	heredoc_input = handle_heredoc(current);
	ft_printf("heredoc_input: %s\n", heredoc_input);
	if (!heredoc_input)
		return (1);
	if (*current_cmd)
	{
		(*current_cmd)->input = heredoc_input;
	}
	else
	{
		free(heredoc_input);
	}
	return (0);
}
