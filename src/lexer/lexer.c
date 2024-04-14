/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:45:34 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/12 16:01:43 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	lexer(char *input, t_token **tokens, t_lexer *lexer) // lexer function takes the input string, a pointer to the tokens and a pointer to the lexer
{
	int		result; // return value

	lexer = malloc(sizeof(t_lexer)); // allocate memory for the lexer
	if (lexer == NULL) // check if the allocation was successful
		return (1); // return 1 if the allocation failed
	lexer->i = 0; //index of the input string , character being processed
	lexer->token_count = 0; //number of tokens
	lexer->buf_index = 0; //index of the buffer where the lexer stores the characters2
	lexer->in_quote = 0; //flag for quotes
	lexer->quote_error = false; //flag for quote errors
	lexer->lexer_error = false; //flag for lexer errors
	lexer->buffer = malloc((ft_strlen(input) + 1) * sizeof(char)); // allocate memory for the buffer
	if (lexer->buffer == NULL)
		return (1);
	result = process_input_loop(input, // process the input string
			&lexer->buffer, &tokens, lexer, &lexer->quote_error);
	return (finalize_lexer(&lexer->buffer, // finalize the lexer and return the result 
			&tokens, lexer,
			lexer->quote_error));
}
