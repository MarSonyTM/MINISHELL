/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 11:16:13 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/08 12:25:58 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	T_R_OT, //token redirect out 
	TOKEN_DOUBLE_REDIRECT_OUT,
	TOKEN_WHITESPACE,
	TOKEN_QUOTE,
	TOKEN_DQUOTE,
	TOKEN_ENV_VAR,
	TOKEN_EXIT_STATUS,
	TOKEN_HEREDOC,
	T_R_OUT_A, //token redirect out append
	TOKEN_COMMA,
	TOKEN_ERROR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next; // For linked list structure
}	t_token;

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
	char			**cmd_arr; //holds flags and arguments
	char			**env_vars; //for env var expansion, else NULL
	char			*exit_status_token; //for exit status expansion, else NULL
	char			*input; //for input redirection, else NULL
	char			*output; //for output redirection, else NULL
	char			*redirection_append; //output redirection append, else NULL
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




typedef struct s_command
{
	t_token	**current;
	t_cmd	**current_cmd;
	t_cmd	***cmd;
	t_env	**env;
	int		*arg_count;
}	t_command;


typedef struct s_lexer
{
	t_env	*env;
	int		i;
	char	*buffer;
	int		*token_count;
	int		buf_index;
	int		in_quote;
	bool	quote_error;
}	t_lexer;


/*Functions prototypes for Lexer*/

int				lexer(char *input, t_token **tokens, t_lexer *lexer);
void			free_tokens(t_token **tokens);
int				add_token(t_token **tokens, t_token_type type, char *value);
t_token_type	determine_token_type(char *token, int inQuote,
					t_lexer *lexer);
t_token_type	handle_first_token(char *token, t_lexer *lexer);
t_token_type	handle_subsequent_tokens(char *token, int inQuote,
					t_lexer *lexer);
t_token_type	check_special_tokens(char *token);
t_token_type	handle_dollar_tokens(char *token, int inQuote);
bool			is_command(char *token, t_lexer *lexer);
char			*construct_full_path(char *dir, char *token);
bool			is_whitespace(char c);
void			process_quotes(char currentChar,
					char **buffer, t_lexer *lexer, int *inQuote);
int				process_whitespace(char *buffer,
					t_token ***tokens, t_lexer *lexer);
int				process_pipe(char *buffer,
					t_token ***tokens, t_lexer *lexer);
int				process_comma(char *buffer,
					t_token ***tokens, t_lexer *lexer);
int				process_heredoc(char **buffer,
					t_token ***tokens, t_lexer *lexer );
int				process_single_redirect_out(char *buffer,
					t_token ***tokens, t_lexer *lexer);
int				process_redirect_out_append(char *buffer,
					t_token ***tokens, t_lexer *lexer, int inQuote);
int				process_single_redirect_in(char *buffer,
					t_token ***tokens, t_lexer *lexer, int inQuote);
int				handle_builtin_or_command(t_cmd **cmd,
					t_token *current, t_cmd **current_cmd, int *arg_count);
int				finalize_buffer_and_add_token(char **buffer, t_token ***tokens, t_lexer *lexer, char *tokenValue, int inQuote);
int				finalize_lexer(char **buffer, t_token ***tokens, t_lexer *lexer, int inQuote, bool quote_error);
int				process_input_loop(char *input, char **buffer,  t_token ***tokens, t_lexer *lexer, int *inQuote, bool *quote_error);
void			process_dollar_conditions(char *input, char **buffer, t_token ***tokens, t_lexer *lexer, int inQuote);

/*Functions prototypes for Parser*/

int				parse(t_token *tokens, t_cmd **cmd, t_env *env);
void			free_cmds(t_cmd **cmd);
char			*resolve_command_path(char *command, t_env *env);
t_cmd			*new_command(t_cmd **cmd);
t_cmd			*initialize_new_command(t_cmd **cmd,
					t_token *current_token, t_env *env);
int				add_argument_to_command(t_cmd *current_cmd,
					const char *arg_value);
int				handle_redirection(t_cmd *current_cmd,
					t_token **current, int current_type);
char			*handle_heredoc(t_token **current);
int				handle_environment_variable(t_cmd *current_cmd, char *value);
int				handle_exit_status_token(t_cmd *current_cmd,
					char *value, int *arg_count);
t_cmd			*handle_pipe_token(t_token **current,
					t_cmd **cmd, t_env *env, int *arg_count);
int				process_tokens(t_token *tokens, t_cmd **cmd, t_env *env);
int				handle_argument(t_cmd *current_cmd, t_token *current);
int				handle_input(t_cmd *current_cmd, t_token *current);
int				handle_parser_redirection(t_cmd *current_cmd,
					t_token **current);
int				handle_parser_heredoc(t_cmd **current_cmd, t_token **current);
int				handle_comma(t_cmd *current_cmd, t_token *current);
int				handle_exit_status(t_cmd *current_cmd, t_token *current);
void			link_command_to_list(t_cmd **cmd_list, t_cmd *new_cmd);
t_cmd			*new_command(t_cmd **cmd);
char			*append_line_to_heredoc(char *heredoc_input,
					const char *input_buffer);
char			*handle_heredoc(t_token **current);
char			*resolve_command_path(char *command, t_env *env);
void			process_token(t_command *command);
int				handle_builtin_or_command_parser(t_command *command);

/*Functions prototypes for Execution*/

void			custom_exec(t_cmd *cmd, t_env **env);
int				executor(t_cmd *cmd, t_env **env);
t_env			*arr_to_linked_list(char **envp);

/* customs */
void			echo_cmd(t_cmd *cmd);
int				cd_cmd(t_cmd *cmd, t_env *env);
int				pwd_cmd(void);
int				env_cmd(t_cmd *cmd, t_env *env);
void			unset_cmd(t_cmd *cmd, t_env **env);
int				exit_cmd(t_cmd *cmd, t_env *env);
int				export_cmd(t_cmd *cmd, t_env **env);

/* error management */
void			clean_up(t_cmd *cmd, t_env *env);
void			close_fds(int *open_fds, int processes);
void			close_and_free(t_exec *exec);
void			error(char *msg, char *ft);

/* utils */
int				redirection(char *file, int mode, int custom);
char			**env_to_array(t_cmd *cmd, t_env **env);
int				get_len(t_env *env);
int				count_processes(t_cmd *cmd);
char			*get_key(char *cmd, int *j);
char			*get_value(char *cmd, int *j);
char			*get_value_concat(char *cmd, int *j);
int				handle_custom(t_cmd *cmd, t_env **env, t_exec *exec, int i);
int				handle_pipe(t_exec *exec, int i, char *cmd_path);
int				duplicate_fd(int old_fd, int new_fd, int custom);
char			*ft_getenv(const char *name, t_env *env);

/* signal management */
void			handle_sigint(int sig);
void			handle_sigquit(int sig);
void			check_blocked_signals(void);

/* expansion */
void			expand_env_vars(t_cmd *cmd, t_env *env);

#endif