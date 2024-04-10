/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:29:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/10 13:15:16 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_character(char current_char, char *input,
	char **buffer,
	t_token ***tokens, t_lexer *lexer)
{
	if (is_whitespace(current_char) && lexer->in_quote == 0)
		process_whitespace(*buffer, tokens, lexer);
	else if (current_char == '|')
		process_pipe(*buffer, tokens, lexer);
	else if (current_char == '\'' || current_char == '\"')
		process_quotes(current_char, buffer, lexer);
	else if (current_char == '<' && lexer->in_quote == 0)
{
    if (input[lexer->i + 1] == '<' && !ft_isalnum((input[lexer->i + 2])))
    {
        if (input[lexer->i + 2] == '<')
            return (2); // Syntax error
        process_heredoc(buffer, tokens, lexer);
    }
    else if (input[lexer->i + 1] != '<' && !ft_isalnum(input[lexer->i + 1]))
    {
        if (input[lexer->i + 1] == '<')
        {
            return (2); // Syntax error
        }
        process_single_redirect_in(*buffer, tokens, lexer);
    }
}
	else if (current_char == '>' && input[lexer->i + 1] != '>' && lexer->in_quote == 0)
		process_single_redirect_out(*buffer,
			tokens, lexer);
	else if (current_char == '$')
		process_dollar_conditions(input, buffer,
			tokens, lexer);
	else if (current_char == '>' && input[lexer->i + 1] == '>' && lexer->in_quote == 0)
		{
		if (input[lexer->i + 1] == '>' && !ft_isspace((input[lexer->i + 2])))
		{
			return (2);
		}
		process_redirect_out_append(*buffer,
			tokens, lexer);
		}
	else
		(*buffer)[(lexer->buf_index)++] = current_char;
	return (0);
}

int	process_input_loop(char *input, char **buffer,
	t_token ***tokens, t_lexer *lexer,
		bool *quote_error)
{
	char	current_char;
	int		result;

	current_char = input[lexer->i];
	while (current_char != '\0' && !(*quote_error))
	{
		result = process_character(current_char, input, buffer,
			tokens, lexer);
		if (result == 2)
		{
			return (2);
			break ;
		}
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
