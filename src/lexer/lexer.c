/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:45:34 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/08 10:27:04 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	lexer(char *input, t_token **tokens, t_env *env)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	if (lexer == NULL)
		return (1);
	lexer->i = 0;
	lexer->token_count = 0;
	lexer->buf_index = 0;
	lexer->in_quote = 0;
	lexer->quote_error = false;
	lexer->buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
	if (lexer->buffer == NULL)
		return (1);
	process_input_loop(input, &lexer->buffer,
		&tokens, lexer, env,
		 &lexer->in_quote, &lexer->quote_error);
	return (finalize_lexer(&lexer->buffer,
			&tokens, lexer,
			lexer->in_quote, lexer->quote_error, env));
}
