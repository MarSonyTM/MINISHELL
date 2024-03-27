#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <sys/wait.h>
# include <fcntl.h>

#define PROMPT "minishell> "

/* error messages */
# define ERROR "Error"
# define ERR_FIL "no such file or directory"
# define ERR_CMD "command not found"
# define ERR_PERM "permission denied"
# define ERR_INT "interrupted system call"
# define ERR_ARG "invalid argument"
# define ERR_ARGS "too many arguments"
# define ERR_ADDR "bad address"
# define ERR_PARS "syntax error near unexpected token"
# define ERR_NUM "numeric argument required"
# define ERR_VAL "not a valid identifier"

/* holds information of each separate token */

typedef enum e_token_type
{
    TOKEN_COMMAND,
    TOKEN_BUILTIN,
    TOKEN_ARG,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_INPUT,
    TOKEN_OUTPUT,
    TOKEN_REDIRECT_OUT,
    TOKEN_DOUBLE_REDIRECT_OUT,
    TOKEN_WHITESPACE,
    TOKEN_QUOTE,
    TOKEN_DQUOTE,
    TOKEN_ENV_VAR,
    TOKEN_EXIT_STATUS,
    TOKEN_HEREDOC,
    TOKEN_REDIRECT_OUT_APPEND,    
    TOKEN_COMMA,
    TOKEN_ERROR
    // Add more as needed
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next; // For linked list structure
} t_token;

/* holds information of each separate environment variable */
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

/* holds information of each separate command / child process */
typedef struct s_cmd
{
    char            *cmd_path; //for execve, else NULL
    char            **cmd_arr; //holds flags and arguments
    char            **env_vars; //for env var expansion, else NULL
    char            *exit_status_token; //for exit status expansion, else NULL
    char            *input; //for input redirection, else NULL
    char            *output; //for output redirection, else NULL
    char            *redirection_append; //for output redirection append, else NULL
    int             exit_status;
    struct s_cmd    *next;
}   t_cmd;

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

/*Functions prototypes for Lexer*/

int     lexer(char *input, t_token **tokens, t_env *env);
void	free_tokens(t_token **tokens);
int 	add_token(t_token **tokens, t_token_type type, char *value);
t_token_type determine_token_type(char *token, int inQuote, t_env *env, int TokenCount);

/*Functions prototypes for Parser*/

int parse(t_token *tokens, t_cmd **cmd, t_env *env);
void free_cmds(t_cmd **cmd);


/*Functions prototypes for Execution*/

void	custom_exec(t_cmd *cmd, t_env **env);
int	    executor(t_cmd *cmd, t_env **env, int exit_status);
t_env	*arr_to_linked_list(char **envp);

/* customs */
void	echo_cmd(t_cmd *cmd);
int	    cd_cmd(t_cmd *cmd, t_env *env);
int 	pwd_cmd(void);
int 	env_cmd(t_cmd *cmd, t_env *env);
void	unset_cmd(t_cmd *cmd, t_env **env);
int 	exit_cmd(t_cmd *cmd, t_env *env);
int 	export_cmd(t_cmd *cmd, t_env **env);

/* error management */
void	clean_up(t_cmd *cmd, t_env *env);
void	close_fds(int *open_fds, int processes);
void	close_and_free(t_exec *exec);
void	error(char *msg, char *command, char *argument, int custom);
void	free_array(char **arr);

/* utils */
int	    redirection(t_cmd *cmd, int mode, int custom);
char	**env_to_array(t_cmd *cmd, t_env **env);
int		get_len(t_env *env);
int		count_processes(t_cmd *cmd);
char	*get_key(char *cmd, int *j);
char	*get_value(char *cmd, int *j);
char	*get_value_concat(char *cmd, int *j);
int 	handle_custom(t_cmd *cmd, t_env **env, t_exec *exec, int i);
int	    handle_pipe(t_exec *exec, int i, char *cmd_path);
int 	duplicate_fd(int old_fd, int new_fd, int custom);
char    *ft_getenv(const char *name, t_env *env);
t_env	*add_env_node(t_env **env, char *key, char *value);
void	add_empty_env_var(char *cmd, t_env **env);
void	add_new_env_var(char *cmd, t_env **env, int j);
void	concatenate_env_var(char *cmd, t_env **env, int j);
void	handle_export_args(t_cmd *cmd, t_env **env, int i);
int     get_last_exit_status(t_cmd *cmd, t_exec *exec);
int	    allocate_memory(t_exec *exec, t_cmd *cmd, t_env **env);
void	handle_fds(t_exec *exec, int i);
void	free_cmds(t_cmd **cmd);
void	reset_free_cmd(t_cmd **cmd, t_token **tokens, char *input);
void	check_args(int argc, char **argv);

/* signal management */
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void check_blocked_signals();

/* expansion */
void expand_env_vars(t_cmd *cmd, t_env *env);

#endif