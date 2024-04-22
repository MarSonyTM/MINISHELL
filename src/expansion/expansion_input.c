/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_input.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 19:50:27 by marianfurni       #+#    #+#             */
/*   Updated: 2024/04/22 12:13:59 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_dollar_special_cases(t_expansion *exp,
		int in_double_quote)
{
	if (**exp->cursor == '\0' || ft_isspace(**exp->cursor)
		|| **exp->cursor == '$' || (in_double_quote
			&& (!ft_isalnum_expansion(**exp->cursor) && **exp->cursor != '_')))
	{
		*exp->result = append_to_string(*exp->result, "$");
		if (**exp->cursor != '\0'
			&& **exp->cursor != '\"' && **exp->cursor != '\'')
			(*exp->cursor)++;
	}
}

static void	handle_dollar_normal_case(t_expansion *exp, t_env *env)
{
	char	*end;
	char	*var_name;
	char	*var_value;

	end = *exp->cursor;
	while (*end && (ft_isalnum_expansion(*end) || *end == '_'))
	{
		end++;
	}
	var_name = ft_strndup(*exp->cursor, end - *exp->cursor);
	var_value = get_env_value(var_name, env);
	free(var_name);
	if (var_value)
	{
		*exp->result = append_to_string(*exp->result, var_value);
		free(var_value);
	}
	*exp->cursor = end;
}

void	handle_dollar(t_expansion *exp,
        int in_single_quote, int in_double_quote, t_env *env)
{
    if (**exp->cursor == '$')
    {
        if (!in_single_quote) {
            (*exp->cursor)++;
            if (**exp->cursor == '?') {
                *exp->result = append_to_string(*exp->result, "$?");
                (*exp->cursor)++;
            } else {
                handle_dollar_special_cases(exp, in_double_quote);
                handle_dollar_normal_case(exp, env);
            }
        } else {
            *exp->result = append_to_string(*exp->result, "$");
            (*exp->cursor)++;
        }
    }
}

void	handle_space(t_expansion *exp,
			int in_single_quote, int in_double_quote)
{
	if (**exp->cursor == ' ' && (in_double_quote || in_single_quote))
	{
		*exp->result = append_to_string(*exp->result, " ");
		(*exp->cursor)++;
	}
}

void	handle_normal_char(t_expansion *exp)
{
	char	append[2];

	append[0] = **exp->cursor;
	append[1] = '\0';
	*exp->result = append_to_string(*exp->result, append);
	(*exp->cursor)++;
}