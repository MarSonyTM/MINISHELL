/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:26:16 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/08 10:30:20 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_heredoc(char **buffer,
	t_token ***tokens, t_lexer *lexer, t_env *env)
{
	if (lexer->buf_index > 0)
	{
		(*buffer)[lexer->buf_index] = '\0';
		if (add_token(*tokens,
				determine_token_type(*buffer, 0, env, lexer),
				ft_strdup(*buffer)) == 1)
			return (1);
		lexer->buf_index = 0;
		(lexer->token_count)++;
	}
	if (add_token(*tokens, TOKEN_HEREDOC, strdup("<<")) == 1)
		return (1);
	(lexer->token_count)++;
	(lexer->i)++;
	return (0);
}
