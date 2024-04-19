/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_cleaners.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 18:42:27 by marianfurni       #+#    #+#             */
/*   Updated: 2024/04/19 18:46:31 by marianfurni      ###   ########.fr       */
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
