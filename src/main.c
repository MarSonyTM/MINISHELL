#include "../inc/minishell.h"

int	handle_lexer(int lexer_status, t_token **tokens, char **input)
{
	printf("lexer_status: %d\n", lexer_status);
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
	printf("parse_status: %d\n", parse_status);
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

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_env		*env;
	t_token		*tokens;
	int			exit_status;
	t_cmd		*cmd;
	t_lexer		lexer_instance;

	exit_status = 0;
	check_args(argc, argv);
	init_env_signals(&env, envp);
	while (1)
	{
		main_handle_input(&input);
		tokens = NULL;
		if (handle_lexer(lexer(input, &tokens,
					&lexer_instance), &tokens, &input))
			continue ;
		cmd = NULL;
		if (handle_parser(parse(tokens, &cmd, env), &cmd, &tokens, &input))
		{
			exit_status = 127;
			continue ;
		}
		expand_env_vars(cmd, env);
		exit_status = executor(cmd, &env, exit_status);
		reset_free_cmd(&cmd, &tokens, input);
	}
	return (0);
}
