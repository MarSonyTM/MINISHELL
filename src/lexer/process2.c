/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:29:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/12 13:34:51 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_redirects(char current_char,
	char *input, char **buffer, t_token ***tokens, t_lexer *lexer)
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
		process_single_redirect_in(*buffer, tokens, lexer);
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
		process_single_redirect_out(*buffer, tokens, lexer);
	}
	return (0);
}

int	process_double_redirects(char current_char,
		char *input, char **buffer, t_token ***tokens, t_lexer *lexer)
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
		process_heredoc(buffer, tokens, lexer);
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
		process_redirect_out_append(*buffer, tokens, lexer);
	}
	return (0);
}

int	process_character(char current_char, char *input,      // function to process the current character
		char **buffer, t_token ***tokens, t_lexer *lexer)
{
	lexer->lexer_error = false; // set lexer error flag to false
	if (is_whitespace(current_char) && lexer->in_quote == 0) // check if the current character is a whitespace character and not in a quote
		process_whitespace(*buffer, tokens, lexer);
	else if (current_char == '|')
		process_pipe(*buffer, tokens, lexer);
	else if (current_char == '\'' || current_char == '\"')
		process_quotes(current_char, buffer, lexer);
	else if (current_char == '<' || current_char == '>')
	{
		if (process_redirects(current_char, input, buffer, tokens, lexer) == 2)
			return (2);
		if (process_double_redirects(current_char,
				input, buffer, tokens, lexer) == 2)
			return (2);
	}
	else if (current_char == '$')
		process_dollar_conditions(input, buffer, tokens, lexer);
	else
		(*buffer)[(lexer->buf_index)++] = current_char;
	return (0);
}

int	process_input_loop(char *input, char **buffer,  // process the input string
	t_token ***tokens, t_lexer *lexer,
		bool *quote_error)
{
	char	current_char; // current character
	int		result; // return value
 
	lexer->lexer_error = false; // set lexer error flag to false
	current_char = input[lexer->i]; // get the first character of the input string
	while (current_char != '\0' && !(*quote_error) && !lexer->lexer_error) // loop through the input string until the end of the string or an error is encountered
	{
		result = process_character(current_char, input, buffer, // process the current character and store it in the buffer
				tokens, lexer);
		(lexer->i)++; // increment the index of the input string
		current_char = input[lexer->i]; // get the next character of the input string
	}
	if (lexer->in_quote != 0) // check if there is an unclosed quote
    {
        error("Unclosed quote", ERROR, NULL, 0);
        *quote_error = true; // set the quote error flag to true
    }
	return (result); // return the result
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
