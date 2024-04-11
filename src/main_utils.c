/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 13:31:59 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/11 13:32:00 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_cmds(t_cmd **cmd)
{
	t_cmd	*current;
	t_cmd	*temp;
	int		i;

	i = 0;
	current = *cmd;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp->cmd_path);
		free_array(temp->cmd_arr);
		i = 0;
		free_array(temp->env_vars);
		free(temp->input);
		free(temp->output);
		free(temp);
	}
	*cmd = NULL;
}

void	reset_free_cmd(t_cmd **cmd, t_token **tokens, char *input)
{
	t_cmd	*current;

	current = *cmd;
	while (current != NULL)
	{
		current->input = NULL;
		current->output = NULL;
		current->redirection_append = NULL;
		current->cmd_path = NULL;
		current = current->next;
	}
	free_tokens(tokens);
	free(input);
	free_cmds(cmd);
}

void	check_args(int argc, char **argv)
{
	if (argc > 1 || argv[1] != NULL)
	{
		error("this program does not take any arguments", "minishell", NULL, 0);
		exit(1);
	}
}
