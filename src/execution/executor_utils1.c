#include "../../inc/minishell.h"

static int	handle_redirect(int fd, int mode, t_cmd *cmd, int custom)
{
	if (fd == -1)
	{
		if (mode == 0)
			error(ERR_FIL, cmd->cmd_arr[0], cmd->input, custom);
		else if (mode == 1)
			error(ERR_PERM, cmd->cmd_arr[0], cmd->output, custom);
		else
			error(ERR_PERM, cmd->cmd_arr[0], cmd->redirection_append, custom);
		return (1);
	}
	if (mode == 0)
	{
		if (dup2(fd, 0) == -1)
			return (1);
	}
	else
	{
		if (dup2(fd, 1) == -1)
			return (1);
	}
	close(fd);
	return (0);
}

int	redirection(t_cmd *cmd, int mode, int custom)
{
	int	fd;

	if (mode == 0)
		fd = open(cmd->input, O_RDONLY);
	else if (mode == 1)
		fd = open(cmd->output, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(cmd->redirection_append, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (handle_redirect(fd, mode, cmd, custom) == 1)
	{
		if (!custom)
			exit(1);
		return (1);
	}
	return (0);
}

int	get_last_exit_status(t_cmd *cmd, t_exec *exec)
{
	t_cmd	*tmp;
	int		last_exit_status;
	int		i;

	last_exit_status = 0;
	i = 0;
	tmp = cmd;
	while (i < exec->processes)
	{
		if (exec->pid[i] != -1)
		{
			waitpid(exec->pid[i], &exec->status[i], 0);
			if (WIFEXITED(exec->status[i]) && WEXITSTATUS(exec->status[i]) != 0)
				last_exit_status = WEXITSTATUS(exec->status[i]);
			else
				last_exit_status = 0;
		}
		if (tmp->exit_status != 0)
			last_exit_status = tmp->exit_status;
		tmp = tmp->next;
		i++;
	}
	return (last_exit_status);
}

int	allocate_memory(t_exec *exec, t_cmd *cmd, t_env **env)
{
	exec->pid = (int *)malloc(sizeof(int) * exec->processes);
	if (!exec->pid)
	{
		clean_up(cmd, *env);
		return (1);
	}
	exec->status = (int *)malloc(sizeof(int) * exec->processes);
	if (!exec->status)
	{
		free(exec->pid);
		clean_up(cmd, *env);
		return (1);
	}
	exec->open_fds = (int *)malloc(sizeof(int) * exec->processes * 2);
	if (!exec->open_fds)
	{
		free(exec->pid);
		free(exec->status);
		clean_up(cmd, *env);
		return (1);
	}
	return (0);
}

void	handle_fds(t_exec *exec, int i)
{
	if (exec->fd[1] != -1)
		close(exec->fd[1]);
	exec->open_fds[i * 2 + 1] = -1;
	if (i != 0)
	{
		if (exec->old_fd[0] != -1)
			close(exec->old_fd[0]);
		exec->open_fds[i * 2 - 2] = -1;
	}
	exec->old_fd[0] = exec->fd[0];
	exec->old_fd[1] = exec->fd[1];
}