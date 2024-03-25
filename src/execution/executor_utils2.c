#include "../../inc/minishell.h"

int	duplicate_fd(int old_fd, int new_fd, int custom)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		if (!custom)
			exit(1);
		return (1);
	}
	close (old_fd);
	return (0);
}

int	check_for_redirect(t_cmd *cmd)
{
	if (cmd->input && redirection(cmd, 0, 1) == 1)
		return (1);
	if (cmd->output && redirection(cmd, 1, 1) == 1)
		return (1);
	if (cmd->redirection_append && redirection(cmd, 2, 1) == 1)
		return (1);
	return (0);
}

int	handle_custom(t_cmd *cmd, t_env **env, t_exec *exec, int i)
{
	int	stdout_fd;

	exec->pid[i] = -1;
	stdout_fd = dup(1);
	if (stdout_fd == -1)
		return (1);
	if (check_for_redirect(cmd) == 1)
		return (1);
	if (cmd->next != NULL)
	{
		if (duplicate_fd(exec->fd[1], 1, 1) == 1)
			return (1);
	}
	custom_exec(cmd, env);
	if (duplicate_fd(stdout_fd, 1, 1) == 1)
		return (1);
	close (stdout_fd);
	if (cmd->next != NULL)
	{
		close(exec->fd[1]);
		exec->open_fds[i * 2 + 1] = -1;
	}
	exec->old_fd[0] = exec->fd[0];
	exec->old_fd[1] = exec->fd[1];
	return (0);
}

int	handle_pipe(t_exec *exec, int i, char *cmd_path)
{
	if (pipe(exec->fd) == -1)
	{
		if (cmd_path != NULL)
			exit (1);
		return (1);
	}
	exec->open_fds[i * 2] = exec->fd[0];
	exec->open_fds[i * 2 + 1] = exec->fd[1];
	return (0);
}

int	count_processes(t_cmd *cmd)
{
	int	processes;

	processes = 0;
	if (cmd == NULL)
		return (processes);
	while (cmd->next != NULL)
	{
		cmd = cmd->next;
		processes++;
	}
	return (processes + 1);
}
