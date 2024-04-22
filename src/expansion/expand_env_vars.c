/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 19:34:47 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/22 12:17:11 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handle_quotes(t_expansion *exp,
				int *in_single_quote, int *in_double_quote)
{
	if (**exp->cursor == '\"')
	{
		*in_double_quote = !(*in_double_quote);
		*exp->result = append_to_string(*exp->result, "\"");
		(*exp->cursor)++;
	}
	else if (**exp->cursor == '\'')
	{
		*in_single_quote = !(*in_single_quote);
		*exp->result = append_to_string(*exp->result, "'");
		(*exp->cursor)++;
	}
}

void	expand_env_varss(t_env *env, t_expansion *exp, char **input)
{
	int		in_single_quote;
	int		in_double_quote;

	*exp->result = NULL;
	*exp->cursor = *input;
	in_single_quote = 0;
	in_double_quote = 0;
	while (**exp->cursor)
	{
		handle_quotes(exp, &in_single_quote, &in_double_quote);
		handle_dollar(exp, in_single_quote, in_double_quote, env);
		handle_space(exp, in_single_quote, in_double_quote);
		handle_normal_char(exp);
	}
	free(*input);
	*input = *exp->result;
}
