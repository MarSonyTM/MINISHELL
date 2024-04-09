/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:49:41 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/09 11:13:31 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*prompt_and_read_line(void)
{
	ft_putstr_fd("> ", STDOUT_FILENO);
	return (readline(NULL));
}

char	*append_line_to_heredoc(char *heredoc_input, const char *input_buffer)
{
	char	*temp;
	char	*new_heredoc_input;

	if (heredoc_input)
		temp = ft_strjoin(heredoc_input, "\n");
	else
		temp = ft_strdup("");
	new_heredoc_input = ft_strjoin(temp, input_buffer);
	free(temp);
	return (new_heredoc_input);
}

char	*handle_heredoc(t_token **current)
{
	char	*delimiter;
	char	*heredoc_input;
	char	*input_buffer;
	char	*new_heredoc_input;

	if (!current || !(*current))
		return (NULL);
	delimiter = (*current)->value;
	heredoc_input = NULL;
	while (1)
	{
		input_buffer = prompt_and_read_line();
		if (!input_buffer)
			return (NULL);
		if (ft_strcmp(input_buffer, delimiter) == 0)
		{
			free(input_buffer);
			break ;
		}
		new_heredoc_input = append_line_to_heredoc(heredoc_input, input_buffer);
		free(heredoc_input);
		heredoc_input = new_heredoc_input;
		free(input_buffer);
	}
	return (heredoc_input);
}
