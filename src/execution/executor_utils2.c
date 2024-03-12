#include "../../inc/minishell.h"

void	duplicate_fd(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		/* error ERROR*/
	}
	close (old_fd);
}

void	handle_custom(t_cmd *cmd, t_env **env, t_exec *exec, int i)
{
	int	stdout_fd;

	exec->pid[i] = -1;
	stdout_fd = dup(1);
	if (cmd->output)
		redirection(cmd->output, 1);
	if (cmd->next != NULL)
		duplicate_fd(exec->fd[1], 1);
	custom_exec(cmd, env);
	duplicate_fd(stdout_fd, 1);
	close (stdout_fd);
	if (cmd->next != NULL)
	{
		close(exec->fd[1]);
		exec->open_fds[i * 2 + 1] = -1;
	}
	exec->old_fd[0] = exec->fd[0];
	exec->old_fd[1] = exec->fd[1];
}

void	handle_pipe(t_exec *exec, int i)
{
	if (pipe(exec->fd) == -1)
	{
		/* error ERROR */
	}
	exec->open_fds[i * 2] = exec->fd[0];
	exec->open_fds[i * 2 + 1] = exec->fd[1];
}

int	count_processes(t_cmd *cmd)
{
	int	processes;

	processes = 0;
	while (cmd->next != NULL)
	{
		cmd = cmd->next;
		processes++;
	}
	return (processes + 1);
}
