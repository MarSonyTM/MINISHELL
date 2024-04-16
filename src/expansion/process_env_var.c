/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_env_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 20:32:46 by marianfurni       #+#    #+#             */
/*   Updated: 2024/04/16 12:04:31 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_env_var(char *var_start, t_cmd *cmd, t_env *env)
{
	char	*var_end;
	char	*var_name;
	char	*var_value;

	var_end = ft_strchr(var_start, '$');
	while (var_end != NULL)
	{
		var_name = ft_strndup(var_start, var_end - var_start);
		var_value = get_env_value(var_name, env);
		if (var_value != NULL)
		{
			append_to_cmd_arr(&cmd->cmd_arr, var_value);
			free(var_value);
		}
		free(var_name);
		var_start = var_end + 1;
		var_end = ft_strchr(var_start, '$');
	}
	var_value = get_env_value(var_start, env);
	if (var_value != NULL)
	{
		append_to_cmd_arr(&cmd->cmd_arr, var_value);
		free(var_value);
	}
}
