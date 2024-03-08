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
#define SYNTAX_ERROR  2
#define CMD_NOT_FOUND 127

/* holds information of each separate token */

typedef enum e_token_type
{
    TOKEN_COMMAND,
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
    char            *input; //for input redirection, else NULL
    char            *output; //for output redirection, else NULL
    int             exit_status;
    struct s_cmd    *next;
}   t_cmd;

/*Functions prototypes for Lexer*/

void	lexer(char *input, t_token **tokens);
void	free_tokens(t_token **tokens);
void	add_token(t_token **tokens, t_token_type type, char *value);
t_token_type determine_token_type(char *token);

/*Functions prototypes for Parser*/

void parse(t_token *tokens, t_cmd **cmd);
void free_cmds(t_cmd **cmd);


#endif