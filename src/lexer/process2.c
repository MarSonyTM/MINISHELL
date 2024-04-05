/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:29:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/05 12:14:39 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	process_character(char current_char, char *input,
	char **buffer, int *bufIndex,
	t_token ***tokens, t_lexer *lexer, t_env *env, int *i, int *inQuote)
{
	if (is_whitespace(current_char) && *inQuote == 0)
		process_whitespace(*buffer, bufIndex, tokens, lexer, env);
	else if (current_char == '|')
		process_pipe(*buffer, bufIndex, tokens, lexer, env);
	else if (current_char == '\'' || current_char == '\"')
		process_quotes(current_char, buffer, bufIndex, inQuote);
	else if (current_char == ',' && *inQuote == 0)
		process_comma(*buffer, bufIndex, tokens, lexer, env);
	else if (current_char == '<' && input[*i + 1] != '<' && *inQuote == 0)
		process_single_redirect_in(*buffer, bufIndex,
			tokens, lexer, env, *inQuote);
	else if (current_char == '>' && input[*i + 1] != '>' && *inQuote == 0)
		process_single_redirect_out(*buffer, bufIndex,
			tokens, lexer, env);
	else if (current_char == '$')
		process_dollar_conditions(input, i, buffer, bufIndex,
			tokens, lexer, env, *inQuote);
	else if (current_char == '<' && input[*i + 1] == '<' && *inQuote == 0)
		process_heredoc(buffer, bufIndex, tokens, lexer, env, i);
	else if (current_char == '>' && input[*i + 1] == '>' && *inQuote == 0)
		process_redirect_out_append(*buffer, bufIndex,
			tokens, lexer, env, i, *inQuote);
	else
		(*buffer)[(*bufIndex)++] = current_char;
}

int	process_input_loop(char *input, char **buffer, int *bufIndex,
	t_token ***tokens, t_lexer *lexer, t_env *env, int *i, int *inQuote,
		bool *quote_error)
{
	char	current_char;

	current_char = input[*i];
	while (current_char != '\0' && !(*quote_error))
	{
		process_character(current_char, input, buffer,
			bufIndex, tokens, lexer, env, i, inQuote);
		(*i)++;
		current_char = input[*i];
	}
	return (0);
}

int	finalize_lexer(char **buffer, int bufIndex, t_token ***tokens,
	t_lexer *lexer, int inQuote, bool quote_error, t_env *env)
{
	if (bufIndex > 0 && !quote_error)
	{
		if (inQuote != 0)
		{
			error(ERR_QUOT, NULL);
			quote_error = true;
		}
		else
		{
			(*buffer)[bufIndex] = '\0';
			if (add_token(*tokens,
					determine_token_type(*buffer, inQuote, env, lexer),
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
