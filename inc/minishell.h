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
# define ERR_ARGS "arg list too long"
# define ERR_ADDR "bad address"
# define ERR_QUOT "unclosed quote"
# define ERR_PARS "syntax error near unexpected token"

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
t_token_type determine_token_type(char *token, int inQuote, t_env *env, int *TokenCount);
// Helper functions
t_token_type handle_first_token(char *token, int *TokenCount);
t_token_type handle_subsequent_tokens(char *token, int inQuote, t_env *env, int *TokenCount);
t_token_type check_special_tokens(char *token);
t_token_type handle_dollar_tokens(char *token, int inQuote);
bool is_command(char *token, t_env *env);
char *construct_full_path(char *dir, char *token);
int process_whitespace(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env);
bool is_whitespace(char c);
int process_pipe(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env);
void process_quotes(char currentChar, char **buffer, int *bufIndex, int *inQuote);
int process_comma(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env);
int finalize_buffer_and_add_token(char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, char *tokenValue, int inQuote);
void process_dollar_conditions(char *input, int *i, char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int inQuote);
int process_heredoc(char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i);
int process_single_redirect_out(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env);
int process_redirect_out_append(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int inQuote);
int process_single_redirect_in(char *buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int inQuote);
int finalize_lexer(char **buffer, int bufIndex, t_token ***tokens, int *TokenCount, int inQuote, bool quote_error, t_env *env);
int process_input_loop(char *input, char **buffer, int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env, int *i, int *inQuote, bool *quote_error);


/*Functions prototypes for Parser*/

int parse(t_token *tokens, t_cmd **cmd, t_env *env);
void free_cmds(t_cmd **cmd);
char *resolve_command_path(char *command, t_env *env);
t_cmd *new_command(t_cmd **cmd);
t_cmd *initialize_new_command(t_cmd **cmd, t_token *current_token, t_env *env);


/*Functions prototypes for Execution*/

void	custom_exec(t_cmd *cmd, t_env **env);
int		executor(t_cmd *cmd, t_env **env);
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
void	error(char *msg, char *ft);

/* utils */
int 	redirection(char *file, int mode, int custom);
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

/* signal management */
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void check_blocked_signals();

/* expansion */
void expand_env_vars(t_cmd *cmd, t_env *env);

#endif