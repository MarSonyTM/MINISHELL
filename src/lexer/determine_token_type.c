/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   determine_token_type.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 17:06:07 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/14 14:43:04 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token_type	 determine_token_type(char *token,   // function that determines the type of the token
				t_lexer *lexer)
{
	if (lexer->token_count == 0) // if the token count is 0, it's the first token
		return (handle_first_token(token, lexer)); 	// handle the first token
	return (handle_subsequent_tokens(token, lexer)); // handle the subsequent tokens
}

t_token_type	handle_first_token(char *token, t_lexer *lexer) // function that handles the first token , has a special case for builtins
{
	int			i;
	char		*builtins[8];

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(token, builtins[i]) == 0)
		{
			(lexer->token_count)++;
			return (TOKEN_BUILTIN);
		}
		i++;
	}
	(lexer->token_count)++;
	return (TOKEN_COMMAND); // if the token is not a builtin, it's a command
}

t_token_type	handle_subsequent_tokens(char *token,   // function that handles the subsequent tokens
				t_lexer *lexer)
{
	t_token_type	special_token;   // special token type
 
	special_token = check_special_tokens(token); // check if the token is a special token
	if (special_token != TOKEN_ARG) // if the token is not an argument
		return (special_token); // return the special token
	if (lexer->token_count == 0 && is_command(token, lexer))  // if the token count is 0 and the token is a command
		return (TOKEN_COMMAND);
	if (token[0] == '$' && token[1] != '\0') // if the token starts with a dollar sign
		return (handle_dollar_tokens(token, lexer));
	return (TOKEN_ARG);
}

t_token_type	check_special_tokens(char *token)
{
	if (ft_strcmp(token, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(token, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(token, ">") == 0)
		return (T_R_OT);
	if (ft_strcmp(token, ">>") == 0)
		return (T_R_OUT_A);
	if (ft_strcmp(token, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(token, ",") == 0)
		return (TOKEN_COMMA);
	if (ft_strcmp(token, "..") == 0)
		return (TOKEN_ARG);
	return (TOKEN_ARG);
}

t_token_type	handle_dollar_tokens(char *token, t_lexer *lexer) // function that handles the dollar tokens
{
	if (lexer->in_quote == 2 || lexer->in_quote == 1)   // if the in_quote flag is set to 2 or 1
		return (TOKEN_ARG); // return the argument token
	if (ft_strcmp(token, "$?") == 0)  // if the token is "$?"
		return (TOKEN_EXIT_STATUS); // return the exit status token
	return (TOKEN_ENV_VAR); // return the environment variable token
}
