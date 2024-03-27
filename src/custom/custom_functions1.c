#include "../../inc/minishell.h"

void	echo_cmd(t_cmd *cmd)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (cmd->cmd_arr[1] && ft_strncmp(cmd->cmd_arr[1], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->cmd_arr[i] != NULL)
	{
		if (ft_strncmp(cmd->cmd_arr[i], "$?", 3) == 0)
			ft_putstr_fd(ft_itoa(cmd->exit_status), 1);
		else
			ft_putstr_fd(cmd->cmd_arr[i], 1);
		if (cmd->cmd_arr[i + 1] != NULL)
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (newline == 1)
		ft_putchar_fd('\n', 1);
}

int	cd_cmd(t_cmd *cmd, t_env *env)
{
	if (cmd->cmd_arr[1] == NULL)
	{
		if (chdir(ft_getenv("HOME", env)) == -1)
			return (1);
	}
	else if (chdir(cmd->cmd_arr[1]) == -1)
	{
		error(ERR_FIL, cmd->cmd_arr[0]);
		return (1);
	}
	return (0);
}

int	pwd_cmd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
		return (1);
	ft_putstr_fd(pwd, 1);
	ft_putchar_fd('\n', 1);
	free(pwd);
	return (0);
}

int	env_cmd(t_cmd *cmd, t_env *env)
{
	t_env	*tmp;

	if (cmd->cmd_arr[1] != NULL)
	{
		error(ERR_ARGS, cmd->cmd_arr[0]);
		return (1);
	}
	tmp = env;
	while (tmp != NULL)
	{
		ft_putstr_fd(tmp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putendl_fd(tmp->value, 1);
		tmp = tmp->next;
	}
	return (0);
}
