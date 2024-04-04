/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_command_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:44 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/04 11:49:39 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*resolve_command_path(char *command, t_env *env)
{
    char *path = ft_getenv("PATH", env); // Get the PATH environment variable value
    char *pathCopy = ft_strdup(path); // Duplicate since strtok modifies the string
    if (!pathCopy)
        return (NULL); // Handle memory allocation failure
    char *dir = ft_strtok(pathCopy, ":");
    size_t commandLen = ft_strlen(command);

    while (dir != NULL) 
    {
        size_t dirLen = ft_strlen(dir);
        char *fullPath = malloc(dirLen + commandLen + 2); // For '/' and '\0'
        if (fullPath == NULL) {
            // Handle memory allocation failure
            free(pathCopy);
            return (NULL);
        }
        
        ft_strcpy(fullPath, dir);
        fullPath[dirLen] = '/'; // Append '/'
        ft_strcpy(fullPath + dirLen + 1, command); // Append command
        if (access (fullPath, F_OK) == 0 && access(fullPath, X_OK) == 0) 
        {
            free(pathCopy);
            return fullPath; // Command found
        }
        free(fullPath); // Free the allocated memory for fullPath
        dir = ft_strtok(NULL, ":");
    }
    free(pathCopy);  
    error(ERR_CMD, command); // Command not found
    return (NULL); // Command not found
}

static t_cmd	*initialize_command_structure(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_path = NULL;
	cmd->cmd_arr = malloc(sizeof(char *) * 2);
	if (!cmd->cmd_arr)
		return (NULL);
	cmd->cmd_arr[0] = NULL;
	cmd->input = NULL;
	cmd->exit_status_token = NULL;
	cmd->env_vars = malloc(sizeof(char *));
	if (!cmd->env_vars)
		return (NULL);
	cmd->env_vars[0] = NULL;
	cmd->output = NULL;
	cmd->exit_status = 0;
	cmd->next = NULL;
	return (cmd);
}

void	link_command_to_list(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*last;

	if (*cmd_list == NULL)
		*cmd_list = new_cmd;
	else
	{
		last = *cmd_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_cmd;
	}
}

t_cmd	*new_command(t_cmd **cmd)
{
	t_cmd	*new_cmd;

	new_cmd = initialize_command_structure();
	if (!new_cmd || !new_cmd->cmd_arr || !new_cmd->env_vars)
	{
		if (new_cmd)
		{
			free(new_cmd->cmd_arr);
			free(new_cmd->env_vars);
		}
		free(new_cmd);
		return (NULL);
	}
	link_command_to_list(cmd, new_cmd);
	return (new_cmd);
}


