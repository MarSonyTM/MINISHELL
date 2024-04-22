/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:40:04 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/22 14:32:08 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_whitespace(char *buffer,
	t_token ***tokens, t_lexer *lexer)
{
	char	*tmp;

	if (lexer->buf_index > 0)
	{
		lexer->in_quote = 0;
		buffer[lexer->buf_index] = '\0';
		tmp = ft_strdup(buffer);
		if (add_token(*tokens, determine_token_type(buffer,
					lexer), tmp) == 1)
			return (free(tmp), 1);
		lexer->buf_index = 0;
		lexer->token_count++;
		free(tmp);
	}
	return (0);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	process_pipe(char *buffer, t_token ***tokens, t_lexer *lexer)
{
	char	*tmp_buffer;
	char	*tmp;

	lexer->in_quote = 0;
	buffer[lexer->buf_index] = '\0';
	tmp_buffer = ft_strdup(buffer);
	if (lexer->buf_index > 0)
	{
		if (add_token(*tokens, determine_token_type(buffer,
					lexer), tmp_buffer) == 1)
			return (free(tmp_buffer), 1);
		lexer->buf_index = 0;
	}
	free(tmp_buffer);
	tmp = ft_strdup("|");
	if (add_token(*tokens, TOKEN_PIPE, tmp) == 1)
		return (free(tmp), 1);
	free(tmp);
	lexer->token_count = 0;
	return (0);
}

void	process_quotes(char currentChar,
			char **buffer, t_lexer *lexer)
{
	if (lexer->in_quote == 0)
	{
		if (currentChar == '\'')
			lexer->in_quote = 1;
		else
			lexer->in_quote = 2;
	}
	else if ((lexer->in_quote == 1 && currentChar == '\'')
		|| (lexer->in_quote == 2 && currentChar == '\"'))
	{
		lexer->in_quote = 0;
		(*buffer)[lexer->buf_index++] = ' ';
	}
	else
		(*buffer)[lexer->buf_index++] = currentChar;
}
