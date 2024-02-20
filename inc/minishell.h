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


/* holds information of each separate command / child process */
typedef struct s_command {
    char *cmd_path;
    char **cmd_args;
    char *input_file;
    char *output_file;
    int append_output;
    struct s_command *next;
} t_command;


#endif