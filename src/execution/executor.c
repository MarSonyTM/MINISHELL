#include "../../inc/minishell.h"

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

static void	create_child_process(t_cmd *cmd, int i, t_exec *exec, t_env **env)
{
	if (cmd->next != NULL)
		handle_pipe(exec, i);
	if (cmd->cmd_path == NULL) //if command is custom
	{
		handle_custom(cmd, env, exec, i);
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

static void	init_exec(t_exec *exec, t_cmd *cmd, t_env **env)
{
	int	i;

	exec->processes = count_processes(cmd);
	exec->old_fd[0] = -1;
	exec->old_fd[1] = -1;
	exec->pid = (int *)malloc(sizeof(int) * exec->processes);
	if (!exec->pid)
		clean_up(cmd, *env);
	exec->status = (int *)malloc(sizeof(int) * exec->processes);
	if (!exec->status)
	{
		free(exec->pid);
		clean_up(cmd, *env);
	}
	exec->open_fds = (int *)malloc(sizeof(int) * exec->processes * 2);
	if (!exec->open_fds)
	{
		free(exec->pid);
		free(exec->status);
		clean_up(cmd, *env);
	}
	i = 0;
	while (i < exec->processes * 2)
		exec->open_fds[i++] = -1;
}

int	executor(t_cmd *cmd, t_env **env)
{
	t_exec	exec;
	int		i;
	int		j;
	int		child_exit_status;

	init_exec(&exec, cmd, env);
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
        int status;
        waitpid(exec.pid[i], &status, 0); // Wait for each child process to finish
        
        if (WIFEXITED(status))
        {
            // Capture the exit status of the child process
            int exitStatus = WEXITSTATUS(status);
            // Optionally, update the exit status for the shell to report
            child_exit_status = exitStatus;
            
            // For debugging: Print the exit status of the child process
            printf("Child process %d exited with status %d\n", exec.pid[i], exitStatus);
        }
    }
    i++;
}
close_and_free(&exec);
// Optionally: Use the exit status of the last child process for some logic
// For now, simply return it or ignore it
return child_exit_status; // Or just return 0 to indicate the shell itself exits normally
}