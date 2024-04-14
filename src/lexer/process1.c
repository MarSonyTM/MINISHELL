/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:40:04 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/14 14:33:05 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_whitespace(char *buffer,   // function that finishes the token and adds it to the tokens array
		t_token ***tokens, t_lexer *lexer)
{
	if (lexer->buf_index > 0) // check if the buffer is not empty
	{
		lexer->in_quote = 0; // set the in_quote flag to 0
		buffer[lexer->buf_index] = '\0'; // set the last character of the buffer to '\0'
		if (add_token(*tokens, determine_token_type(buffer, // add the token to the tokens array
					lexer), strdup(buffer)) == 1)
			return (1);
		lexer->buf_index = 0; // reset the buffer index
		lexer->token_count++; // increment the token count
	}
	return (0);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	process_pipe(char *buffer, t_token ***tokens, t_lexer *lexer)
{
	lexer->in_quote = 0;
	buffer[lexer->buf_index] = '\0';
	if (lexer->buf_index > 0)
	{
		if (add_token(*tokens, determine_token_type(buffer,
					lexer), ft_strdup(buffer)) == 1)
			return (1);
		lexer->buf_index = 0;
	}
	if (add_token(*tokens, TOKEN_PIPE, ft_strdup("|")) == 1)
		return (1);
	lexer->token_count = 0;
	return (0);
}

void	process_quotes(char currentChar,  // function that processes quotes
		char **buffer, t_lexer *lexer)
{
	if (lexer->in_quote == 0) // check if the in_quote flag is 0
	{
		if (currentChar == '\'')
			lexer->in_quote = 1;  // if the current character is a single quote, set the in_quote flag to 1
		else
			lexer->in_quote = 2; // if the current character is a double quote, set the in_quote flag to 2
	}
	else if ((lexer->in_quote == 1 && currentChar == '\'') // if the in_quote flag is 1 and the current character is a single quote
		|| (lexer->in_quote == 2 && currentChar == '\"')) // or the in_quote flag is 2 and the current character is a double quote
		lexer->in_quote = 0; // set the in_quote flag to 0
	else
		(*buffer)[lexer->buf_index++] = currentChar; // add the current character to the buffer
}
