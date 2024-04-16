/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:29:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/16 11:44:04 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_redirects(char current_char,
	char *input, t_token ***tokens, t_lexer *lexer)
{
	if (current_char == '<'
		&& input[lexer->i + 1] != '<' && lexer->in_quote == 0)
	{
		if (input[lexer->i + 1] == '>')
		{
			lexer->lexer_error = true;
			error("Syntax error near unexpected token `>'", ERROR, NULL, 0);
			return (2);
		}
		process_single_redirect_in(lexer->buffer, tokens, lexer);
	}
	else if (current_char == '>'
		&& input[lexer->i + 1] != '>' && lexer->in_quote == 0)
	{
		if (input[lexer->i + 1] == '<')
		{
			lexer->lexer_error = true;
			error("Syntax error near unexpected token `<'", ERROR, NULL, 0);
			return (2);
		}
		process_single_redirect_out(lexer->buffer, tokens, lexer);
	}
	return (0);
}

int	process_double_redirects(char current_char,
			char *input, t_token ***tokens, t_lexer *lexer)
{
	if (current_char == '<'
		&& input[lexer->i + 1] == '<' && lexer->in_quote == 0)
	{
		if (input[lexer->i + 1] == '<' && !ft_isalnum((input[lexer->i + 2])))
		{
			lexer->lexer_error = true;
			error("Syntax error near unexpected token", ERROR, NULL, 0);
			return (2);
		}
		process_heredoc(&(lexer->buffer), tokens, lexer);
	}
	else if (current_char == '>'
		&& input[lexer->i + 1] == '>' && lexer->in_quote == 0)
	{
		if (input[lexer->i + 1] == '>' && !ft_isspace((input[lexer->i + 2])))
		{
			lexer->lexer_error = true;
			error("Syntax error near unexpected token", ERROR, NULL, 0);
			return (2);
		}
		process_redirect_out_append(lexer->buffer, tokens, lexer);
	}
	return (0);
}

int	process_character(char current_char, char *input,
		t_token ***tokens, t_lexer *lexer)
{
	lexer->lexer_error = false;
	if (is_whitespace(current_char) && lexer->in_quote == 0)
		process_whitespace(lexer->buffer, tokens, lexer);
	else if (current_char == '|')
		process_pipe(lexer->buffer, tokens, lexer);
	else if (current_char == '\'' || current_char == '\"')
		process_quotes(current_char, &(lexer->buffer), lexer);
	else if (current_char == '<' || current_char == '>')
	{
		if (process_redirects(current_char,
				input, tokens, lexer) == 2)
			return (2);
		if (process_double_redirects(current_char,
				input, tokens, lexer) == 2)
			return (2);
	}
	else if (current_char == '$')
		process_dollar_conditions(input, &(lexer->buffer), tokens, lexer);
	else
		lexer->buffer[(lexer->buf_index)++] = current_char;
	return (0);
}

int	process_input_loop(char *input, t_token ***tokens, t_lexer *lexer,
		bool *quote_error)
{
	char	current_char;
	int		result;

	lexer->lexer_error = false;
	current_char = input[lexer->i];
	while (current_char != '\0' && !(*quote_error) && !lexer->lexer_error)
	{
		result = process_character(current_char, input,
				tokens, lexer);
		(lexer->i)++;
		current_char = input[lexer->i];
	}
	if (lexer->in_quote != 0)
	{
		error("Unclosed quote", ERROR, NULL, 0);
		*quote_error = true;
	}
	return (result);
}

int	finalize_lexer(char **buffer, t_token ***tokens,
	t_lexer *lexer, bool quote_error)
{
	if (lexer->buf_index > 0 && !quote_error)
	{
		if (lexer->in_quote != 0)
		{
			error("Unclosed quote", ERROR, NULL, 0);
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
