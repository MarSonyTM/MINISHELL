#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <sys/wait.h>
# include <fcntl.h>


#define PROMPT "minishell> "

/* defines token types */
typedef enum 
{
    CMD,
    INPUT,
    OUTPUT,
    APPEND_OUTPUT,
    PIPE
} TokenType;

/* holds information of each separate environment variable */
typedef struct s_env
{
    char            *key;
    char            *value;
    struct s_env    *next;
}   t_env;

typedef struct s_lexer
{
    int             cmd_num; // for command number
    TokenType       type; // 0 = cmd, 1 = input, 2 = output , 3 = pipe
    char            *cmd;
    char            *value;
    int             quoted;  // New flag to track if inside a quoted string
    struct s_lexer  *next;
}   t_lexer;


/* holds information of each separate command / child process */
typedef struct s_command {
    char *cmd_path;
    char **cmd_args;
    char *input_file;
    char *output_file;
    int append_output;
    struct s_command *next;
} t_command;

typedef struct s_parse {
    t_command *command;
    int exit_status;
    struct s_parse *next;
} t_parse;

t_lexer *lex(const char *input);
t_parse *parse(t_lexer *tokens);


#endif