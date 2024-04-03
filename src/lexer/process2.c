/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 10:29:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 11:01:23 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_single_redirect_out(char *buffer,
	int *bufIndex, t_token ***tokens, int *TokenCount, t_env *env)
{
	if (*bufIndex > 0)
	{
		buffer[*bufIndex] = '\0';
		if (add_token(*tokens,
				determine_token_type(buffer, 0, env, TokenCount),
				ft_strdup(buffer)) == 1)
			return (1);
		*bufIndex = 0;
	}
	if (add_token(*tokens, TOKEN_REDIRECT_OUT, ft_strdup(">")) == 1)
		return (1);
	(*TokenCount)++;
	return (0);
}

int	process_redirect_out_append(char *buffer, int *bufIndex,
	t_token ***tokens, int *TokenCount, t_env *env, int *i, int inQuote)
{
	if (*bufIndex > 0)
	{
		buffer[*bufIndex] = '\0';
		if (add_token(*tokens,
				determine_token_type(buffer, inQuote, env, TokenCount),
				ft_strdup(buffer)) == 1)
			return (1);
		*bufIndex = 0;
	}
	if (add_token(*tokens, TOKEN_REDIRECT_OUT_APPEND, ft_strdup(">>")) == 1)
		return (1);
	(*TokenCount)++;
	(*i)++;
	return (0);
}

// Helper function to process single character redirection "<"
int	process_single_redirect_in(char *buffer, int *bufIndex,
	t_token ***tokens, int *TokenCount, t_env *env, int inQuote)
{
	if (*bufIndex > 0)
	{
		buffer[*bufIndex] = '\0';
		if (add_token(*tokens,
				determine_token_type(buffer, inQuote, env, TokenCount),
				ft_strdup(buffer)) == 1)
			return (1);
		*bufIndex = 0;
	}
	if (add_token(*tokens, TOKEN_REDIRECT_IN, ft_strdup("<")) == 1)
		return (1);
	(*TokenCount)++;
	return (0);
}

int	process_input_loop(char *input, char **buffer, int *bufIndex,
	t_token ***tokens, int *TokenCount, t_env *env, int *i, int *inQuote,
		bool *quote_error)
{
	char	current_char;

	current_char = input[*i];
	while ((current_char = input[*i]) != '\0' && !(*quote_error))
	{
		if (is_whitespace(current_char) && *inQuote == 0)
			process_whitespace(*buffer, bufIndex, tokens, TokenCount, env);
		else if (current_char == '|')
			process_pipe(*buffer, bufIndex, tokens, TokenCount, env);
		else if (current_char == '\'' || current_char == '\"')
			process_quotes(current_char, buffer, bufIndex, inQuote);
		else if (current_char == ',' && *inQuote == 0)
			process_comma(*buffer, bufIndex, tokens, TokenCount, env);
		else if (current_char == '<' && input[*i + 1] != '<' && *inQuote == 0)
			process_single_redirect_in(*buffer, bufIndex,
				tokens, TokenCount, env, *inQuote);
		else if (current_char == '>' && input[*i + 1] != '>' && *inQuote == 0)
			process_single_redirect_out(*buffer, bufIndex,
				tokens, TokenCount, env);
		else if (current_char == '$')
			process_dollar_conditions(input, i, buffer, bufIndex,
				tokens, TokenCount, env, *inQuote);
		else if (current_char == '<' && input[*i + 1] == '<' && *inQuote == 0)
			process_heredoc(buffer, bufIndex, tokens, TokenCount, env, i);
		else if (current_char == '>' && input[*i + 1] == '>' && *inQuote == 0)
			process_redirect_out_append(*buffer, bufIndex,
				tokens, TokenCount, env, i, *inQuote);
		else
			(*buffer)[(*bufIndex)++] = current_char;
		(*i)++;
	}
	return (0);
}

int	finalize_lexer(char **buffer, int bufIndex, t_token ***tokens,
	int *TokenCount, int inQuote, bool quote_error, t_env *env)
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
					determine_token_type(*buffer, inQuote, env, TokenCount),
					ft_strdup(*buffer)) == 1)
				return (free(*buffer), 1);
			(*TokenCount)++;
		}
	}
	if (quote_error)
		return (free(*buffer), 2);
	free(*buffer);
	return (0);
}
