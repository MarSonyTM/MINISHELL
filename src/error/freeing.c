#include "../../inc/minishell.h"

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	clean_up(t_cmd *cmd, t_env *env)
{
	t_cmd	*tmp;
	t_env	*env_tmp;
	int		exit_status;

	exit_status = 0;
	while (cmd != NULL)
	{
		tmp = cmd;
		cmd = cmd->next;
		exit_status = tmp->exit_status;
		free(tmp->cmd_path);
		free(tmp->input);
		free(tmp->output);
		free_array(tmp->cmd_arr);
		free(tmp);
	}
	while (env != NULL)
	{
		env_tmp = env;
		env = env->next;
		free(env_tmp->key);
		free(env_tmp->value);
		free(env_tmp);
	}
	// exit(exit_status);
}
