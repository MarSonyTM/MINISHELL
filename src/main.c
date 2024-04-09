#include "../inc/minishell.h"


int	handle_lexer(int lexer_status, t_token **tokens, char **input)
{
	if (lexer_status == 1)
	{
		free_tokens(tokens);
		free(*input);
		exit(1);
	}
	else if (lexer_status == 2)
	{
		free_tokens(tokens);
		free(*input);
		return (1);
	}
	return (0);
}

int	handle_parser(int parse_status, t_cmd **cmd, t_token **tokens, char **input)
{
	if (parse_status == 1)
	{
		free_cmds(cmd);
		free_tokens(tokens);
		free(*input);
		exit(1);
	}
	else if (parse_status == 2)
	{
		free_cmds(cmd);
		free_tokens(tokens);
		free(*input);
		return (1);
	}
	return (0);
}

void	init_env_signals(t_env **env, char **envp)
{
	*env = arr_to_linked_list(envp);
	if (!*env)
		exit(1);
	check_blocked_signals();
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	main_handle_input(char **input)
{
	*input = readline(PROMPT);
	sleep(0);
	if (!*input)
	{
		free(*input);
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	add_history(*input);
}

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

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_env		*env;
	t_token		*tokens;
	int			exit_status;
	t_cmd		*cmd;

	exit_status = 0;
	check_args(argc, argv);
	init_env_signals(&env, envp);
	while (1)
	{
		main_handle_input(&input);
		tokens = NULL;
		if (handle_lexer(lexer(input, &tokens, env), &tokens, &input))
			continue ;
		t_token *current;
		current = tokens;
		const char	*type_str;
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
				case T_R_OUT_A:
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
		if (handle_parser(parse(tokens, &cmd, env), &cmd, &tokens, &input))
		{
			exit_status = 127;
			continue ;
		}
		print_commands(cmd);
		expand_env_vars(cmd, env);
		exit_status = executor(cmd, &env, exit_status);
		reset_free_cmd(&cmd, &tokens, input);
	}
	return (0);
}