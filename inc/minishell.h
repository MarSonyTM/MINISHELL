#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <sys/wait.h>


#define PROMPT "minishell> "

/* holds overall information like number of processes needed and cmd list */
typedef struct s_parser
{
    int     processes;
    t_cmd   *cmd;
}

/* holds information of each separate command / child process */
typedef struct s_cmd
{
    char    *cmd_path;
    char    **cmd_arr;
    char    *input; //for everything but first command NULL; could also be heredoc
    char    *output; //for everything but last command NULL
    char    **env; //could also be a linked list
    int     exit_status;
    t_cmd    *next;
}   t_cmd;

#endif