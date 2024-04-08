/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 13:32:28 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/08 17:27:57 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_single_redirect_out(char *buffer,
	t_token ***tokens, t_lexer *lexer)
{
	lexer->in_quote = 0;
	if (lexer->buf_index > 0)
	{
		buffer[lexer->buf_index] = '\0';
		if (add_token(*tokens,
				determine_token_type(buffer, lexer),
				ft_strdup(buffer)) == 1)
			return (1);
		lexer->buf_index = 0;
	}
	if (add_token(*tokens, T_R_OT, ft_strdup(">")) == 1)
		return (1);
	(lexer->token_count)++;
	return (0);
}

int	process_redirect_out_append(char *buffer,
	t_token ***tokens, t_lexer *lexer)
{
	if (lexer->buf_index > 0)
	{
		buffer[lexer->buf_index] = '\0';
		if (add_token(*tokens,
				determine_token_type(buffer, lexer),
				ft_strdup(buffer)) == 1)
			return (1);
		lexer->buf_index = 0;
	}
	if (add_token(*tokens, T_R_OUT_A, ft_strdup(">>")) == 1)
		return (1);
	(lexer->token_count)++;
	(lexer->i)++;
	return (0);
}

// Helper function to process single character redirection "<"
int	process_single_redirect_in(char *buffer,
	t_token ***tokens, t_lexer *lexer)
{
	if (lexer->buf_index > 0)
	{
		buffer[lexer->buf_index] = '\0';
		if (add_token(*tokens,
				determine_token_type(buffer, lexer),
				ft_strdup(buffer)) == 1)
			return (1);
		lexer->buf_index = 0;
	}
	if (add_token(*tokens, TOKEN_REDIRECT_IN, ft_strdup("<")) == 1)
		return (1);
	(lexer->token_count)++;
	return (0);
}