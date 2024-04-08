/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_dollar_sign.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:29:15 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/08 12:51:18 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	finalize_buffer_and_add_token(char **buffer,
	t_token ***tokens,
	t_lexer *lexer, char *tokenValue)
{
	if (lexer->buf_index > 0)
	{
		(*buffer)[lexer->buf_index] = '\0';
		add_token(*tokens,
			determine_token_type(*buffer, lexer),
			ft_strdup(*buffer));
		lexer->buf_index = 0;
	}
	if (tokenValue != NULL)
	{
		if (add_token(*tokens,
				determine_token_type(tokenValue, lexer),
				ft_strdup(tokenValue)) == 1)
			return (1);
	}
	(lexer->token_count)++;
	return (0);
}

void	process_dollar_conditions(char *input, char **buffer,
		t_token ***tokens, t_lexer *lexer)
{
	if (input[lexer->i + 1] == '?')
	{
		finalize_buffer_and_add_token(buffer,
			tokens, lexer, "$?");
		lexer->i += 1;
	}
	else if (input[lexer->i + 1] == '\0' || !ft_isalpha(input[lexer->i + 1]))
	{
		finalize_buffer_and_add_token(buffer, tokens,
			lexer, "$");
	}
	else
	{
		(*buffer)[(lexer->buf_index)++] = input[lexer->i];
	}
}
