/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/04/22 10:25:53 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

void	handle_input_and_expansion(t_env **env, t_main_loop *loop)
{
	main_handle_input(&loop->input);
	loop->cursor = loop->input;
	loop->result = NULL;
	loop->exp.cursor = &loop->cursor;
	loop->exp.result = &loop->result;
	expand_env_varss(*env, &loop->exp, &loop->input);
}

int	handle_lexer_and_parser(t_env **env, t_main_loop *loop, int *exit_status)
{
	loop->tokens = NULL;
	if (handle_lexer(lexer(loop->input,
				&loop->tokens, &loop->lexer_instance),
			&loop->tokens, &loop->input))
		return (1);
	loop->cmd = NULL;
	if (handle_parser(parse(loop->tokens,
				&loop->cmd, *env), &loop->cmd, &loop->tokens, &loop->input))
	{
		return (1);
	}
	return (0);
}

void	main_loop(t_env **env, int *exit_status)
{
	t_main_loop	loop;

	while (1)
	{
		handle_input_and_expansion(env, &loop);
		if (handle_lexer_and_parser(env, &loop, exit_status))
		{
			*exit_status = 127;
			continue ;
		}
		free_tokens(&loop.tokens);
		*exit_status = executor(loop.cmd, env, *exit_status);
		reset_free_cmd(&loop.cmd, loop.input);
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
