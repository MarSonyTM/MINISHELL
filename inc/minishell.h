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

typedef  struct s_lexer
{
    int             cmd_num; // for command number
    TokenType       type; //0 = cmd, 1 = input, 2 = output , 3 = pipe
    char            *cmd;
    char            *value;
    struct s_lexer  *next;
}   t_lexer;

/* holds information of each separate command / child process */
typedef struct s_parse
{
    char            *cmd_path; //for execve, else NULL
    char            **cmd_arr; //holds flags and arguments
    char            *input; //for input redirection, else NULL
    char            *output; //for output redirection, else NULL 
    int             exit_status;
    struct s_parse  *next;
}   t_parse;


t_lexer *lex(const char *input);
t_parse *parse(t_lexer *tokens);


#endif