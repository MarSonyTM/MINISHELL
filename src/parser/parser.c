/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:53:08 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/14 20:33:48 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Main parse function
int	parse(t_token *tokens, t_cmd **cmd, t_env *env) // tokens is a pointer to a struct of tokens, cmd is a pointer to a pointer to a struct of commands, env is a pointer to a struct of environment variables
{
	if (!tokens) // if tokens is NULL
		return (2); // return 2 
	return (process_tokens(tokens, cmd, env)); // return the result of the process_tokens function
	return (0); // return 0
}


int	process_tokens(t_token *tokens, t_cmd **cmd, t_env *env) 
{
	t_command	command; // declare a struct of type t_command
	t_cmd		*current_cmd; // declare a pointer to a struct of type t_cmd
	int			arg_count; // declare an integer variable arg_count
	t_token		*current; // declare a pointer to a struct of type t_token

	current_cmd = NULL; 
	arg_count = 0; 
	current = tokens;	       
	command.current = &current;
	command.current_cmd = &current_cmd;
	command.cmd = &cmd;
	command.env = &env;
	command.arg_count = &arg_count;
	command.err_code = 0;
	while (current != NULL)
	{
		process_token(&command);        
		if (command.err_code != 0)
			return (command.err_code);
		if (!current_cmd) 
			return (1); 
		current = current->next;
	}
	return (0);
}
