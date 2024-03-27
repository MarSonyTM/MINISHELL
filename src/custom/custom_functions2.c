#include "../../inc/minishell.h"

static int	check_for_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			error(ERR_NUM, "exit", str, 1);
			return (0);
		}
		i++;
	}
	return (1);
}

int	exit_cmd(t_cmd *cmd, t_env *env)
{
	int	exit_code;

	exit_code = EXIT_SUCCESS;
	if (cmd->cmd_arr[1] && cmd->cmd_arr[2])
	{
		error(ERR_ARGS, "exit", NULL, 1);
		return (1);
	}
	else if (cmd->cmd_arr[1] && check_for_digit(cmd->cmd_arr[1]))
	{
		exit_code = ft_atoi(cmd->cmd_arr[1]);
		while (exit_code < 0)
			exit_code = 256 + exit_code;
		if (exit_code > 255)
			exit_code = exit_code % 256;
	}
	clean_up(cmd, env);
	ft_putstr_fd("exit\n", 1);
	exit(exit_code);
	return (0);
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
