#include "../../inc/minishell.h"

static void	duplicate_fd(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		/* error ERROR*/
	}
	close (old_fd);
}

static void	child_process(t_cmd *cmd, int i, t_exec *exec, t_env **env)
{
	char	**envp;

	if (i != exec->processes - 1) //if this is not the last process
		duplicate_fd(exec->fd[1], 1); //duplicate write end of pipe to stdout
	if (i != 0) //if this is not the first process
	{
		duplicate_fd(exec->old_fd[0], 0); //duplicate read end of old pipe to stdin
		close(exec->fd[0]); //close read end of pipe
		exec->open_fds[i * 2] = -1;
	}
	if (cmd->input)
		redirection(cmd->input, 0);
	if (cmd->output)
		redirection(cmd->output, 1);
	envp = env_to_array(cmd, env);
	if (execve(cmd->cmd_path, cmd->cmd_arr, envp) == -1)
	{
		/* error ERROR*/
		exit (1);
	}
}

/* 	Only the last custom command is being executed!
	So, no possibility of two customs after each other.
	It makes no sense to have a custom in the middle of a pipeline, since it takes no input.
	But it does work! Just ignores what came before.
	So, no need to have the read end of a pipe for a custom.
	
	In conclusion: custom does not read, but can write into pipe.
	It can be at the beginning, middle or end of a pipeline. */

static void	create_child_process(t_cmd *cmd, int i, t_exec *exec, t_env **env)
{
	int	stdout_fd;

	if (cmd->next != NULL)
	{
		if (pipe(exec->fd) == -1)
			exit(1);
		exec->open_fds[i * 2] = exec->fd[0];
		exec->open_fds[i * 2 + 1] = exec->fd[1];
	}
	if (cmd->cmd_path == NULL) //if command is custom
	{
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
		return ;
	}
	exec->pid[i] = fork();
	if (exec->pid[i] == -1)
		exit(1);
	if (exec->pid[i] == 0) //if this is the child process
	{
		child_process(cmd, i, exec, env);
		exit(0);
	}
	close (exec->fd[1]); //close write end of pipe
	exec->open_fds[i * 2 + 1] = -1;
	if (i != 0) //if this is not the first process
	{
		close(exec->old_fd[0]); //close read end of old pipe
		exec->open_fds[i * 2 - 2] = -1;
	}
	exec->old_fd[0] = exec->fd[0]; //set old read end of pipe to current read end of pipe
	exec->old_fd[1] = exec->fd[1]; //set old write end of pipe to current write end of pipe
}

/* how to keep track of amount of non-custom commands? and how to wait for them? */

int	executor(t_cmd *cmd, t_env **env)
{
	t_exec	exec;
	int		i;
	int		j;

	exec.processes = count_processes(cmd);
	exec.old_fd[0] = -1;
	exec.old_fd[1] = -1;
	exec.pid = (int *)malloc(sizeof(int) * exec.processes);
	if (!exec.pid)
		clean_up(cmd, *env);
	exec.status = (int *)malloc(sizeof(int) * exec.processes);
	if (!exec.status)
	{
		free(exec.pid);
		clean_up(cmd, *env);
	}
	exec.open_fds = (int *)malloc(sizeof(int) * exec.processes * 2);
	if (!exec.open_fds)
	{
		free(exec.pid);
		free(exec.status);
		clean_up(cmd, *env);
	}
	i = 0;
	while (i < exec.processes * 2)
	{
		exec.open_fds[i] = -1;
		i++;
	}
	i = 0; //initialize i to 0, siginifies process number
	j = exec.processes; //initialize j to number of processes
	while (j > 0) //while there are still processes to execute
	{
		create_child_process(cmd, i, &exec, env);
		j--;
		i++;
		if (cmd->next != NULL) //if there are still commands to execute
			cmd = cmd->next; //move to next command
	}
	i = 0;
	while (i < exec.processes)
	{
		if (exec.pid[i] != -1)
		{
			waitpid(exec.pid[i], &exec.status[i], 0);
			if (WIFEXITED(exec.status[i]) && WEXITSTATUS(exec.status[i]) != 0)
				exit(1);
		}
		i++;
	}
	free(exec.pid);
	free(exec.status);
	if (exec.processes > 1)
	{
		close(exec.old_fd[0]);
		exec.open_fds[(exec.processes - 1) * 2 - 2] = -1;
		close(exec.old_fd[1]);
		exec.open_fds[(exec.processes - 1) * 2 - 1] = -1;
	}
	close_fds(exec.open_fds, exec.processes);
	return (0);
}
