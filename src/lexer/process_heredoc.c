/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:26:16 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 10:28:44 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_heredoc(char **buffer, int *bufIndex,
	t_token ***tokens, int *TokenCount, t_env *env, int *i)
{
	if (*bufIndex > 0)
	{
		(*buffer)[*bufIndex] = '\0';
		if (add_token(*tokens,
				determine_token_type(*buffer, 0, env, TokenCount),
				ft_strdup(*buffer)) == 1)
			return (1);
		*bufIndex = 0;
		(*TokenCount)++;
	}
	if (add_token(*tokens, TOKEN_HEREDOC, strdup("<<")) == 1)
		return (1);
	(*TokenCount)++;
	(*i)++;
	return (0);
}
