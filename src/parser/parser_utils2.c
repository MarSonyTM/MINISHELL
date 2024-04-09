/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:07:12 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/09 12:52:06 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_exit_status_token(t_cmd *current_cmd, char *value, int *arg_count)
{
	if (!current_cmd)
		return (1);
	current_cmd->cmd_arr = realloc(current_cmd->cmd_arr,
			sizeof(char *) * (*arg_count + 2));
	if (!current_cmd->cmd_arr)
		return (1);
	current_cmd->cmd_arr[*arg_count] = ft_strdup(value);
	if (!current_cmd->cmd_arr[*arg_count])
		return (1);
	*arg_count += 1;
	current_cmd->cmd_arr[*arg_count] = NULL;
	return (0);
}

t_cmd	*handle_pipe_token(t_token **current,
	t_cmd **cmd, t_env *env, int *arg_count)
{
	t_cmd	*current_cmd;
	char	*cmd_path;

	*current = (*current)->next;
	if (*current == NULL
		|| ((*current)->type != TOKEN_COMMAND
			&& (*current)->type != TOKEN_BUILTIN))
		return (error(ERR_PARS, "\n", NULL, 0), NULL);
	current_cmd = new_command(cmd);
	if (!current_cmd)
		return (NULL);
	current_cmd->cmd_arr[0] = ft_strdup((*current)->value);
	if (!current_cmd->cmd_arr[0])
		return (NULL);
	current_cmd->cmd_arr[1] = NULL;
	cmd_path = resolve_command_path((*current)->value, env);
	if (!cmd_path)
		return (NULL);
	current_cmd->cmd_path = cmd_path;
	*arg_count = 1;
	return (current_cmd);
}

int	handle_comma(t_cmd *current_cmd, t_token *current)
{
	if (current_cmd == NULL)
		return (0);
	if (add_argument_to_command(current_cmd, current->value) != 0)
		return (1);
	return (0);
}

int	process_token(t_command *command)
{
	t_token	**current;
	t_cmd	**current_cmd;

	current = command->current;
	current_cmd = command->current_cmd;
	if ((*current)->type == TOKEN_BUILTIN || (*current)->type == TOKEN_COMMAND)
		return (handle_builtin_or_command_parser(command));
	else if ((*current)->type == TOKEN_ARG)
		return (handle_argument(*current_cmd, *current));
	else if ((*current)->type == TOKEN_INPUT && *current_cmd != NULL)
		return (handle_input(*current_cmd, *current));
	else if ((*current)->type == T_R_OT || (*current)->type == T_R_OUT_A
		|| (*current)->type == TOKEN_REDIRECT_IN)
		return (handle_parser_redirection(*current_cmd, current));
	else if ((*current)->type == TOKEN_HEREDOC)
		return (handle_parser_heredoc(current_cmd, current));
	else if ((*current)->type == TOKEN_COMMA)
		return (handle_comma(*current_cmd, *current));
	else if ((*current)->type == TOKEN_EXIT_STATUS)
		return (handle_exit_status_token(*current_cmd,
				(*current)->value, command->arg_count));
	else if ((*current)->type == TOKEN_ENV_VAR)
		return (handle_environment_variable(*current_cmd, (*current)->value));
	else if ((*current)->type == TOKEN_PIPE)
		*current_cmd = handle_pipe_token(current,
				*command->cmd, *command->env, command->arg_count);
	return(0);
}

