/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:08 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/09 11:14:58 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Main parse function
int	parse(t_token *tokens, t_cmd **cmd, t_env *env)
{
	return(process_tokens(tokens, cmd, env));
	return (0);
}
