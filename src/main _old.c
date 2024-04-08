#include "../inc/minishell.h"

void	print_commands(t_cmd *cmd)
{
	t_cmd	*current_cmd = cmd;
    int i = 1;
    while (current_cmd != NULL) 
    {
        printf("PARSER: Command: %s\n", current_cmd->cmd_arr[0]); // Print command
        while ( current_cmd->cmd_arr[i] != NULL) 
        {
            printf("PARSER: Arg: %s\n", current_cmd->cmd_arr[i]); // Print arguments
            i++;
        }
        if (current_cmd->input != NULL)
            printf("PARSER: Input Redirection: %s\n", current_cmd->input);
        if (current_cmd->output != NULL)
            printf("PARSER: Output Redirection: %s\n", current_cmd->output);
        if (current_cmd->redirection_append != NULL)
            printf("PARSER: Output Redirection Append: %s\n", current_cmd->redirection_append);
        current_cmd = current_cmd->next; // Move to the next commandr
        i = 1; // Reset argument index for the next command
    }
}

void	free_cmds(t_cmd **cmd)
{
	t_cmd	*current;
	t_cmd	*temp;
	int		i;

	i = 0;
	current = *cmd;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp->cmd_path);
		free_array(temp->cmd_arr);
		i = 0;
		free_array(temp->env_vars);
		free(temp->input);
		free(temp->output);
		free(temp);
	}
	*cmd = NULL;
}

static void	reset_cmd(t_cmd *cmd)
{
	while (cmd != NULL)
	{
		cmd->input = NULL;
		cmd->output = NULL;
		cmd->redirection_append = NULL;
		cmd->cmd_path = NULL;
		cmd = cmd->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	const char	*type_str;
	char		*input;
	t_env		*env;
	t_token		*tokens;
	t_token		*current;
	int			exit_status;
	int			lexer_status;
	t_cmd		*cmd;
	int			parse_status;

	exit_status = 0;
	env = arr_to_linked_list(envp);
	if (!env)
		exit(1);
	if (argc > 1 || argv[1] != NULL)
	{
		error("this program does not take any arguments", "minishell", NULL, 0);
		exit(1);
	}
	check_blocked_signals();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	while (1)
	{
		input = readline(PROMPT);
		sleep(0);
		if (!input)
		{
			free(input);
			ft_putendl_fd("exit\n", 1);
			break ;
		}
		add_history(input);
		tokens = NULL;
		lexer_status = lexer(input, &tokens, env);
		if (lexer_status == 1)
		{
			free_tokens(&tokens);
			free(input);
			exit(1);
		}
		else if (lexer_status == 2)
		{
			free_tokens(&tokens);
			free(input);
			exit_status = 1;
			continue ;
		}
		current = tokens;
		while (current != NULL)
		{
			switch (current->type)
			{
				case TOKEN_ARG:
					type_str = "Argument";
					break;
				case TOKEN_PIPE:
					type_str = "Pipe";
					break;
				case TOKEN_REDIRECT_IN:
					type_str = "Input Redirection";
					break;
				case T_R_OT:
					type_str = "Output Redirection";
					break;
				case TOKEN_QUOTE:
					type_str = "Single Quote";
					break;
				case TOKEN_DQUOTE:
					type_str = "Double Quote";
					break;
				case TOKEN_COMMAND:
					type_str = "Command";
					break;
				case TOKEN_ENV_VAR:
					type_str = "Environment Variable";
					break;
				case TOKEN_EXIT_STATUS:
					type_str = "Exit Status";
					break;
				case TOKEN_HEREDOC:
					type_str = "Heredoc";
					break;
				case TOKEN_COMMA:
					type_str = "Comma";
					break;
				case TOKEN_REDIRECT_OUT_APPEND:
					type_str = "Output Redirection Append";
					break;
				case TOKEN_BUILTIN:
					type_str = "Builtin";
					break;
				default:
					type_str = "Unknown";
					break;
			}
			printf("LEXER: Token: Type: %s, Value: %s\n", type_str, current->value);
			current = current->next;
		}
		cmd = NULL;
		parse_status = parse(tokens, &cmd, env);
		if (parse_status == 1)
		{
			free_cmds(&cmd);
			free_tokens(&tokens);
			free(input);
			exit(1);
		}
		else if (parse_status == 2)
		{
			free_cmds(&cmd);
			free_tokens(&tokens);
			free(input);
			exit_status = 127;
			continue ;
		}
		expand_env_vars(cmd, env);
		print_commands(cmd);
		exit_status = executor(cmd, &env, exit_status);
		reset_cmd(cmd);
		free_tokens(&tokens);
		free(input);
		free_cmds(&cmd);
	}
	return (0);
}
