/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:04:35 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 15:05:31 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Function to resolve the path of a command
t_cmd	*initialize_new_command(t_cmd **cmd, t_token *current_token, t_env *env)
{
	t_cmd	*new_cmd;

	new_cmd = new_command(cmd);
	if (!new_cmd)
		return (NULL);
	new_cmd->cmd_arr = malloc(sizeof(char *) * 2);
	if (!new_cmd->cmd_arr)
		return (NULL);
	new_cmd->cmd_arr[0] = ft_strdup(current_token->value);
	if (!new_cmd->cmd_arr[0])
		return (NULL);
	new_cmd->cmd_arr[1] = NULL;
	if (current_token->type == TOKEN_COMMAND)
	{
		new_cmd->cmd_path = resolve_command_path(current_token->value, env);
		if (!new_cmd->cmd_path)
			return (NULL);
	}
	else
	{
		new_cmd->cmd_path = NULL;
	}
	return (new_cmd);
}


int	add_argument_to_command(t_cmd *current_cmd, const char *arg_value)
{
	int		arg_count;
	char	**temp;

	if (current_cmd == NULL || arg_value == NULL)
		return (1);
	arg_count = 0;
	while (current_cmd->cmd_arr[arg_count] != NULL)
	{
		arg_count++;
	}
	temp = realloc(current_cmd->cmd_arr, sizeof(char *) * (arg_count + 2));
	if (!temp)
		return (1);
	current_cmd->cmd_arr = temp;
	current_cmd->cmd_arr[arg_count] = ft_strdup(arg_value);
	if (current_cmd->cmd_arr[arg_count] == NULL)
		return (1);
	current_cmd->cmd_arr[arg_count + 1] = NULL;
	return (0);
}

int	handle_redirection(t_cmd *current_cmd, t_token **current, int current_type)
{
	if (!current_cmd || !current || !(*current))
		return (1);
	*current = (*current)->next;
	if (!(*current) || !(*current)->value)
	{
		error(ERR_PARS, "\n");
		return (2);
	}
	if (current_type == TOKEN_REDIRECT_OUT_APPEND)
	{
		current_cmd->redirection_append = ft_strdup((*current)->value);
		if (!current_cmd->redirection_append)
			return (1);
	}
	else if (current_type == TOKEN_REDIRECT_OUT)
	{
		current_cmd->output = ft_strdup((*current)->value);
		if (!current_cmd->output)
			return (1);
	}
	return (0);
}


int	handle_environment_variable(t_cmd *current_cmd, char *value)
{
	int		env_var_count;
	char	**temp;

	if (current_cmd == NULL)
		return (1);
	env_var_count = 0;
	while (current_cmd->env_vars
		&& current_cmd->env_vars[env_var_count] != NULL)
	{
		env_var_count++;
	}
	temp = realloc(current_cmd->env_vars, sizeof(char *) * (env_var_count + 2));
	if (!temp)
		return (1);
	current_cmd->env_vars = temp;
	current_cmd->env_vars[env_var_count] = ft_strdup(value);
	if (!current_cmd->env_vars[env_var_count])
		return (1);
	current_cmd->env_vars[env_var_count + 1] = NULL;
	printf("Parser env_var: %s\n", current_cmd->env_vars[env_var_count]);
	return (0);
}

int	handle_exit_status(t_cmd *current_cmd, t_token *current)
{
	if (current_cmd == NULL)
		return (0);
	current_cmd->exit_status_token = ft_strdup(current->value);
	if (!current_cmd->exit_status_token)
		return (1);
	return (0);
}
