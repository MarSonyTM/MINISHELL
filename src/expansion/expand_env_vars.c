/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:34:47 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/18 19:41:51 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_quotes(char **cursor,
				int *in_single_quote, int *in_double_quote, char **result)
{
	if (**cursor == '\"')
	{
		*in_double_quote = !(*in_double_quote);
		*result = append_to_string(*result, "\"");
		(*cursor)++;
	}
	else if (**cursor == '\'')
	{
		*in_single_quote = !(*in_single_quote);
		*result = append_to_string(*result, "'");
		(*cursor)++;
	}
}

void	expand_env_varss(char **input, t_env *env)
{
	char	*result;
	char	*cursor;
	int		in_single_quote;
	int		in_double_quote;

	result = NULL;
	cursor = *input;
	in_single_quote = 0;
	in_double_quote = 0;
	while (*cursor)
	{
		handle_quotes(&cursor, &in_single_quote, &in_double_quote, &result);
		handle_dollar(&cursor, in_single_quote, in_double_quote, &result, env);
		handle_space(&cursor, in_single_quote, in_double_quote, &result);
		handle_normal_char(&cursor, &result);
	}
	free(*input);
	*input = result;
}
