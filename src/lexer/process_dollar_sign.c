/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_dollar_sign.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:29:15 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 10:26:01 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	finalize_buffer_and_add_token(char **buffer, int *bufIndex,
	t_token ***tokens,
	int *TokenCount, t_env *env, char *tokenValue, int inQuote)
{
	if (*bufIndex > 0)
	{
		(*buffer)[*bufIndex] = '\0';
		add_token(*tokens,
			determine_token_type(*buffer, inQuote, env, TokenCount),
			ft_strdup(*buffer));
		*bufIndex = 0;
	}
	if (tokenValue != NULL)
	{
		if (add_token(*tokens,
				determine_token_type(tokenValue, inQuote, env, TokenCount),
				ft_strdup(tokenValue)) == 1)
			return (1);
	}
	(*TokenCount)++;
	return (0);
}

void	process_dollar_conditions(char *input, int *i, char **buffer,
		int *bufIndex,
		t_token ***tokens, int *TokenCount, t_env *env, int inQuote)
{
	if (input[*i + 1] == '?')
	{
		finalize_buffer_and_add_token(buffer, bufIndex,
			tokens, TokenCount, env, "$?", inQuote);
		*i += 1;
	}
	else if (input[*i + 1] == '\0' || !ft_isalpha(input[*i + 1]))
	{
		finalize_buffer_and_add_token(buffer, bufIndex, tokens,
			TokenCount, env, "$", inQuote);
	}
	else
	{
		(*buffer)[(*bufIndex)++] = input[*i];
	}
}
