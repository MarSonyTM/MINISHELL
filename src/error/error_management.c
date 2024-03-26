#include "../../inc/minishell.h"

void	close_and_free(t_exec *exec)
{
	free(exec->pid);
	free(exec->status);
	if (exec->processes > 1)
	{
		close(exec->old_fd[0]);
		exec->open_fds[(exec->processes - 1) * 2 - 2] = -1;
		close(exec->old_fd[1]);
		exec->open_fds[(exec->processes - 1) * 2 - 1] = -1;
	}
	close_fds(exec->open_fds, exec->processes);
}

void	close_fds(int *open_fds, int processes)
{
	int	i;

	i = 0;
	while (i < processes * 2)
	{
		if (open_fds[i] != -1)
			close(open_fds[i]);
		i++;
	}
	free(open_fds);
}

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

	while (cmd != NULL)
	{
		tmp = cmd;
		cmd = cmd->next;
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
}

void	error(char *msg, char *command, char *argument, int custom)
{
	if (custom)
		ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": ", 2);
	if (argument)
	{
		ft_putstr_fd(argument, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}
