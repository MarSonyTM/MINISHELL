/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/04/30 17:11:27 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	main_handle_input(char **input, t_env *env)
{
	*input = readline(PROMPT);
	if (!*input)
	{
		free_env(env);
		free(*input);
		ft_putendl_fd("exit", 1);
		exit(0);
	}
	add_history(*input);
}

void	handle_input_and_expansion(t_env **env, t_main_loop *loop)
{
	main_handle_input(&loop->input, *env);
	loop->cursor = loop->input;
	loop->result = NULL;
	loop->exp.cursor = &loop->cursor;
	loop->exp.result = &loop->result;
	expand_env_varss(*env, &loop->exp, &loop->input);
}

int	handle_lexer_and_parser(t_env **env, t_main_loop *loop)
{
	loop->tokens = NULL;
	if (handle_lexer(lexer(loop->input,
				&loop->tokens, &loop->lexer_instance),
			&loop->tokens, &loop->input, *env))
		return (1);
	loop->cmd = NULL;
	if (handle_parser(parse(loop->tokens,
				&loop->cmd, *env), loop, *env))
	{
		return (1);
	}
	return (0);
}

int	main_loop(t_env **env, int *exit_status)
{
	t_main_loop	loop;

	while (1)
	{
		handle_input_and_expansion(env, &loop);
		if (handle_lexer_and_parser(env, &loop) && !g_signal_caught)
		{
			*exit_status = 127;
			continue ;
		}
		else
		{
			g_signal_caught = 0;
			*exit_status = 130;
		}
		free_tokens(&loop.tokens);
		*exit_status = executor(loop.cmd, env, *exit_status);
		reset_free_cmd(&loop.cmd, loop.input);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_env		*env;
	int			exit_status;

	exit_status = 0;
	check_args(argc, argv);
	init_env_signals(&env, envp);
	signal(SIGQUIT, SIG_IGN);
	main_loop(&env, &exit_status);
	return (0);
}
