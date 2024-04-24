/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:51:56 by csturm            #+#    #+#             */
/*   Updated: 2024/04/24 18:03:41 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdio.h>

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
        i = 1; // Reset argument index for the next comman<<<<<<< HEADd
    }
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

void	handle_input_and_expansion(t_env **env, t_main_loop *loop)
{
	main_handle_input(&loop->input, *env);
	loop->cursor = loop->input;
	loop->result = NULL;
	loop->exp.cursor = &loop->cursor;
	loop->exp.result = &loop->result;
	expand_env_varss(*env, &loop->exp, &loop->input);
	printf("input: %s\n", loop->input);
}

int	handle_lexer_and_parser(t_env **env, t_main_loop *loop)
{
	loop->tokens = NULL;
	if (handle_lexer(lexer(loop->input,
				&loop->tokens, &loop->lexer_instance),
			&loop->tokens, &loop->input, *env))
		return (1);
	t_token *current;
		current = loop->tokens;
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
	loop->cmd = NULL;
	if (handle_parser(parse(loop->tokens,
				&loop->cmd, *env), loop, *env))
	{
		return (1);
	}
	print_commands(loop->cmd);
	return (0);
}

int	main_loop(t_env **env, int *exit_status)
{
	t_main_loop	loop;

	while (1)
	{
		handle_input_and_expansion(env, &loop);
		if (handle_lexer_and_parser(env, &loop))
		{
			*exit_status = 127;
			continue ;
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
	main_loop(&env, &exit_status);
	return (0);
}
