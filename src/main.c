/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/04/19 14:15:59 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	handle_lexer(int lexer_status, t_token **tokens, char **input, t_env *env)
{
	if (lexer_status == 1)
	{
		free_env(env);
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

int	handle_parser(int parse_status, t_cmd **cmd, t_token **tokens, char **input, t_env *env)
{
	if (parse_status == 1)
	{
		clean_up(*cmd, env);
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

void	main_handle_input(char **input, t_env *env)
{
	*input = readline(PROMPT);
	sleep(0);
	if (!*input)
	{
		free_env(env);
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
		main_handle_input(&input, *env);
		tokens = NULL;
		if (handle_lexer(lexer(input, &tokens,
					&lexer_instance), &tokens, &input, *env))
		{
			*exit_status = 1;
			continue ;
		}
		cmd = NULL;
		if (handle_parser(parse(tokens, &cmd, *env), &cmd, &tokens, &input, *env))
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
