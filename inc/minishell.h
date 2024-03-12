#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <sys/wait.h>
# include <fcntl.h>

# define PROMPT "minishell> "

/* error messages */
# define ERROR "Error"
# define ERR_FIL "No such file or directory"
# define ERR_CMD "Command not found"
# define ERR_PERM "Permission denied"
# define ERR_INT "Interrupted system call"
# define ERR_ARG "Invalid argument"
# define ERR_ARGS "Arg list too long"
# define ERR_ADDR "Bad address"

/* holds information of each separate environment variable */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* holds information of each separate command / child process */
typedef struct s_cmd
{
	char			*cmd_path; //for execve, else NULL
	char			**cmd_arr; //holds command, flags and arguments
	char			*input; //for input redirection, else NULL
	char			*output; //for output redirection, else NULL
	int				exit_status;
	struct s_cmd	*next;
}	t_cmd;

/* just reduces variables in execution */
typedef struct s_exec
{
	int	fd[2];
	int	old_fd[2];
	int	processes;
	int	*pid;
	int	*status;
	int	*open_fds;
	int	non_customs;
}	t_exec;

void	custom_exec(t_cmd *cmd, t_env **env);
int		executor(t_cmd *cmd, t_env **env);
t_env	*arr_to_linked_list(char **envp);

/* customs */
void	echo_cmd(t_cmd *cmd);
void	cd_cmd(t_cmd *cmd);
void	pwd_cmd(void);
void	env_cmd(t_cmd *cmd, t_env *env);
void	unset_cmd(t_cmd *cmd, t_env **env);
void	exit_cmd(t_cmd *cmd, t_env *env);
void	export_cmd(t_cmd *cmd, t_env **env);

/* error management */
void	clean_up(t_cmd *cmd, t_env *env);
void	close_fds(int *open_fds, int processes);
void	close_and_free(t_exec *exec);

/* utils */
void	redirection(char *file, int mode);
char	**env_to_array(t_cmd *cmd, t_env **env);
int		get_len(t_env *env);
int		count_processes(t_cmd *cmd);
char	*get_key(char *cmd, int *j);
char	*get_value(char *cmd, int *j);
char	*get_value_concat(char *cmd, int *j);
void	handle_custom(t_cmd *cmd, t_env **env, t_exec *exec, int i);
void	handle_pipe(t_exec *exec, int i);
void	duplicate_fd(int old_fd, int new_fd);

#endif