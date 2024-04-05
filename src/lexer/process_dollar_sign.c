/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_dollar_sign.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:29:15 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/05 12:23:34 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	finalize_buffer_and_add_token(char **buffer, int *bufIndex,
	t_token ***tokens,
	t_lexer *lexer, t_env *env, char *tokenValue, int inQuote)
{
	if (*bufIndex > 0)
	{
		(*buffer)[*bufIndex] = '\0';
		add_token(*tokens,
			determine_token_type(*buffer, inQuote, env, lexer),
			ft_strdup(*buffer));
		*bufIndex = 0;
	}
	if (tokenValue != NULL)
	{
		if (add_token(*tokens,
				determine_token_type(tokenValue, inQuote, env, lexer),
				ft_strdup(tokenValue)) == 1)
			return (1);
	}
	(lexer->token_count)++;
	return (0);
}

void	process_dollar_conditions(char *input, int *i, char **buffer,
		int *bufIndex,
		t_token ***tokens, t_lexer *lexer, t_env *env, int inQuote)
{
	if (input[*i + 1] == '?')
	{
		finalize_buffer_and_add_token(buffer, bufIndex,
			tokens, lexer, env, "$?", inQuote);
		*i += 1;
	}
	else if (input[*i + 1] == '\0' || !ft_isalpha(input[*i + 1]))
	{
		finalize_buffer_and_add_token(buffer, bufIndex, tokens,
			lexer, env, "$", inQuote);
	}
	else
	{
		(*buffer)[(*bufIndex)++] = input[*i];
	}
}
