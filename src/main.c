/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/05/01 13:23:20 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"


void	main_handle_input(char **input, t_env *env)
{
	*input = readline(PROMPT);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
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
	expand_env_vars(*env, &loop->exp, &loop->input, loop);
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

int	main_loop(t_env **env)
{
	t_main_loop	loop;
	int			exit_status;

	exit_status = 0;
	loop.exit_status = exit_status;
	while (1)
	{
		setup_signals();
		signal(SIGQUIT, SIG_IGN);
		handle_input_and_expansion(env, &loop);
		g_signal_caught = 0;
		if (handle_lexer_and_parser(env, &loop) && !g_signal_caught)
		{
			loop.exit_status = 127;
			continue ;
		}
		else if (g_signal_caught)
		{
			g_signal_caught = 0;
			printf("signal caught\n");
			loop.exit_status = 130;
			continue ;
		}
		free_tokens(&loop.tokens);
		loop.exit_status = executor(loop.cmd, env, loop.exit_status);
		reset_free_cmd(&loop.cmd, loop.input);
	}
	return (loop.exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env		*env;

	check_args(argc, argv);
	init_env_signals(&env, envp);
	main_loop(&env);
	return (0);
}
