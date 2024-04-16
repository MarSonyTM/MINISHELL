/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/04/16 18:23:23 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	main_loop(t_env **env, int *exit_status)
{
	char		*input;
	t_token		*tokens;
	t_cmd		*cmd;
	t_lexer		lexer_instance;

	while (1)
	{
		main_handle_input(&input);
		tokens = NULL;
		if (handle_lexer(lexer(input, &tokens,
					&lexer_instance), &tokens, &input))
			continue ;
		cmd = NULL;
		if (handle_parser(parse(tokens, &cmd, *env), &cmd, &tokens, &input))
		{
			*exit_status = 127;
			continue ;
		}
		free_tokens(&tokens);
		expand_env_vars(cmd, *env);
		*exit_status = executor(cmd, env, *exit_status);
		reset_free_cmd(&cmd, input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_env		*env;
	int			exit_status;

	exit_status = 0;
	check_args(argc, argv);
	init_env_signals(&env, envp);
	main_loop(&env, &exit_status);
	return (0);
}
