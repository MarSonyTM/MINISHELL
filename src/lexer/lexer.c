/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:45:34 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 10:20:29 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	lexer(char *input, t_token **tokens, t_env *env)
{
	int		i;
	char	*buffer;
	int		token_count;
	int		buf_index;
	int		in_quote;
	bool	quote_error;

	i = 0;
	buffer = malloc((ft_strlen(input) + 1) * sizeof(char));
	token_count = 0;
	buf_index = 0;
	in_quote = 0;
	quote_error = false;
	if (buffer == NULL)
		return (1);
	process_input_loop(input, &buffer, &buf_index,
		&tokens, &token_count, env, &i, &in_quote, &quote_error);
	return (finalize_lexer(&buffer, buf_index,
			&tokens, &token_count, in_quote, quote_error, env));
}
