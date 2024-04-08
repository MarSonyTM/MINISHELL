/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:29:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/08 12:58:06 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_character(char current_char, char *input,
	char **buffer,
	t_token ***tokens, t_lexer *lexer)
{
	if (is_whitespace(current_char) && lexer->in_quote == 0)
		process_whitespace(*buffer, tokens, lexer);
	else if (current_char == '|')
		process_pipe(*buffer, tokens, lexer);
	else if (current_char == '\'' || current_char == '\"')
		process_quotes(current_char, buffer, lexer);
	else if (current_char == '<' && input[lexer->i + 1] != '<' && lexer->in_quote == 0)
		process_single_redirect_in(*buffer,
			tokens, lexer);
	else if (current_char == '>' && input[lexer->i + 1] != '>' && lexer->in_quote == 0)
		process_single_redirect_out(*buffer,
			tokens, lexer);
	else if (current_char == '$')
		process_dollar_conditions(input, buffer,
			tokens, lexer);
	else if (current_char == '<' && input[lexer->i + 1] == '<' && lexer->in_quote == 0)
		process_heredoc(buffer, tokens, lexer);
	else if (current_char == '>' && input[lexer->i + 1] == '>' && lexer->in_quote == 0)
		process_redirect_out_append(*buffer,
			tokens, lexer);
	else
		(*buffer)[(lexer->buf_index)++] = current_char;
}

int	process_input_loop(char *input, char **buffer,
	t_token ***tokens, t_lexer *lexer,
		bool *quote_error)
{
	char	current_char;

	current_char = input[lexer->i];
	while (current_char != '\0' && !(*quote_error))
	{
		process_character(current_char, input, buffer,
			tokens, lexer);
		(lexer->i)++;
		current_char = input[lexer->i];
	}
	return (0);
}

int	finalize_lexer(char **buffer, t_token ***tokens,
	t_lexer *lexer, bool quote_error)
{
	if (lexer->buf_index > 0 && !quote_error)
	{
		if (lexer->in_quote != 0)
		{
			error(ERR_QUOT, NULL);
			quote_error = true;
		}
		else
		{
			(*buffer)[lexer->buf_index] = '\0';
			if (add_token(*tokens,
					determine_token_type(*buffer, lexer),
					ft_strdup(*buffer)) == 1)
				return (free(*buffer), 1);
			(lexer->token_count)++;
		}
	}
	if (quote_error)
		return (free(*buffer), 2);
	free(*buffer);
	return (0);
}
