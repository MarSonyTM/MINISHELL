/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 19:50:27 by marianfurni       #+#    #+#             */
/*   Updated: 2024/04/18 19:44:01 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static void	handle_dollar_special_cases(char **cursor,
		int in_double_quote, char **result)
{
	if (**cursor == '\0' || isspace(**cursor)
		|| **cursor == '$' || (in_double_quote
			&& (!isalnum(**cursor) && **cursor != '_')))
	{
		*result = append_to_string(*result, "$");
		if (**cursor != '\0' && **cursor != '\"' && **cursor != '\'')
			(*cursor)++;
	}
}

static void	handle_dollar_normal_case(char **cursor, char **result, t_env *env)
{
	char	*end;
	char	*var_name;
	char	*var_value;

	end = *cursor;
	while (*end && (isalnum(*end) || *end == '_'))
	{
		end++;
	}
	var_name = ft_strndup(*cursor, end - *cursor);
	var_value = get_env_value(var_name, env);
	free(var_name);
	if (var_value)
	{
		*result = append_to_string(*result, var_value);
		free(var_value);
	}
	*cursor = end;
}

void	handle_dollar(char **cursor,
            int in_single_quote, int in_double_quote, char **result, t_env *env)
{
	if (**cursor == '$' && !in_single_quote)
	{
		(*cursor)++;
		handle_dollar_special_cases(cursor, in_double_quote, result);
		handle_dollar_normal_case(cursor, result, env);
	}
}

void	handle_space(char **cursor,
			int in_single_quote, int in_double_quote, char **result)
{
	if (**cursor == ' ' && (in_double_quote || in_single_quote))
	{
		*result = append_to_string(*result, " ");
		(*cursor)++;
	}
}

void	handle_normal_char(char **cursor, char **result)
{
	char	append[2];

	append[0] = **cursor;
	append[1] = '\0';

	*result = append_to_string(*result, append);
	(*cursor)++;
}


