/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:40:04 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/05 12:15:03 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_whitespace(char *buffer, int *bufIndex,
	t_token ***tokens, t_lexer *lexer, t_env *env)
{
	if (*bufIndex > 0)
	{
		buffer[*bufIndex] = '\0';
		if (add_token(*tokens, determine_token_type(buffer,
					0, env, lexer), strdup(buffer)) == 1)
			return (1);
		*bufIndex = 0;
		lexer->token_count++;
	}
	return (0);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	process_pipe(char *buffer,
	int *bufIndex, t_token ***tokens, t_lexer *lexer, t_env *env)
{
	buffer[*bufIndex] = '\0';
	if (*bufIndex > 0)
	{
		if (add_token(*tokens, determine_token_type(buffer,
					0, env, lexer), ft_strdup(buffer)) == 1)
			return (1);
		*bufIndex = 0;
	}
	if (add_token(*tokens, TOKEN_PIPE, ft_strdup("|")) == 1)
		return (1);
	lexer->token_count = 0;
	return (0);
}

void	process_quotes(char currentChar,
		char **buffer, int *bufIndex, int *inQuote)
{
	if (*inQuote == 0)
	{
		if (currentChar == '\'')
			*inQuote = 1;
		else
			*inQuote = 2;
	}
	else if ((*inQuote == 1 && currentChar == '\'')
		|| (*inQuote == 2 && currentChar == '\"'))
		*inQuote = 0;
	else
		(*buffer)[(*bufIndex)++] = currentChar;
}

int	process_comma(char *buffer, int *bufIndex,
			t_token ***tokens, t_lexer *lexer, t_env *env)
{
	if (*bufIndex > 0)
	{
		buffer[*bufIndex] = '\0';
		if (add_token(*tokens, determine_token_type(buffer,
					0, env, lexer), ft_strdup(buffer)) == 1)
			return (1);
		*bufIndex = 0;
		(lexer->token_count)++;
	}
	if (add_token(*tokens, TOKEN_COMMA, ft_strdup(",")) == 1)
		return (1);
	(lexer->token_count)++;
	return (0);
}
