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
# define ERR_FIL "No such file or directory"
# define ERR_CMD "Command not found"
# define ERR_PERM "Permission denied"
# define ERR_INT "Interrupted system call"
# define ERR_ARG "Invalid argument"
# define ERR_ARGS "Arg list too long"
# define ERR_ADDR "Bad address"

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
    char            *env_var; //for env var expansion, else NULL
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

void	lexer(char *input, t_token **tokens);
void	free_tokens(t_token **tokens);
void	add_token(t_token **tokens, t_token_type type, char *value);
t_token_type determine_token_type(char *token);

/*Functions prototypes for Parser*/

void parse(t_token *tokens, t_cmd **cmd);
void free_cmds(t_cmd **cmd);


/*Functions prototypes for Execution*/

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
int 	handle_custom(t_cmd *cmd, t_env **env, t_exec *exec, int i);
int	    handle_pipe(t_exec *exec, int i, char *cmd_path);
int 	duplicate_fd(int old_fd, int new_fd, int custom);

/* signal management */
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

/* expansion */
void expand_env_vars(t_cmd *cmd);

#endif