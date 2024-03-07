#include "../../inc/minishell.h"

void	exit_cmd(t_cmd *cmd, t_env *env)
{
	int	i;
	int	exit_code;

	i = 0;
	exit_code = EXIT_SUCCESS;
	if (cmd->cmd_arr[1] && cmd->cmd_arr[2])
	{
		/* error ERR_ARGS*/
	}
	else if (cmd->cmd_arr[1])
	{
		while (cmd->cmd_arr[1][i] && ft_isdigit(cmd->cmd_arr[1][i]))
			i++;
		if (cmd->cmd_arr[1][i])
		{
			/* error ERR_ARG*/
		}
		exit_code = ft_atoi(cmd->cmd_arr[1]);
		while (exit_code < 0)
			exit_code = 256 + exit_code;
		if (exit_code > 255)
			exit_code = exit_code % 256;
	}
	clean_up(cmd, env);
	ft_putstr_fd("exit\n", 1);
	exit(exit_code);
}

static void	free_env_node(t_env **env, t_env *tmp, t_env *prev)
{
	if (prev == NULL)
		*env = tmp->next;
	else
		prev->next = tmp->next;
	free(tmp->key);
	free(tmp->value);
	free(tmp);
}

void	unset_cmd(t_cmd *cmd, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;
	int		i;

	i = 1;
	while (cmd->cmd_arr[i] != NULL)
	{
		tmp = *env;
		prev = NULL;
		while (tmp != NULL)
		{
			if (ft_strncmp(tmp->key, cmd->cmd_arr[i],
					ft_strlen(cmd->cmd_arr[i]) + 1) == 0)
			{
				free_env_node(env, tmp, prev);
				break ;
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
}
