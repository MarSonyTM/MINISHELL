/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:49:41 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/09 17:36:32 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_variables(const char *input)
{
	char	*output;

	output = ft_strdup("");
	while (*input)
	{
		if (*input == '$')
			output = expand_variable(&input, output);
		else
		{
			output = ft_strjoin_free_char(output, *input);
			input++;
		}
	}
	return (output);
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

int	create_temp_file(char *temp_file_name, int temp_file_num)
{
	int	fd;

	while (1)
	{
		fd = open(temp_file_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
		if (fd == -1 && errno == EEXIST)
		{
			temp_file_num++;
			temp_file_name[20] = '0' + temp_file_num;
			continue ;
		}
		else if (fd == -1)
		{
			perror("open");
			return (-1);
		}
		break ;
	}
	return (fd);
}

char	*read_and_write_heredoc(int fd, char *delimiter, char *heredoc_input)
{
	char	*input_buffer;
	char	*new_heredoc_input;
	char	*expanded_input;

	while (1)
	{
		input_buffer = prompt_and_read_line();
		if (!input_buffer)
			return (close(fd), free(heredoc_input), NULL);
		if (ft_strcmp(input_buffer, delimiter) == 0)
		{
			free(input_buffer);
			break ;
		}
		expanded_input = expand_variables(input_buffer);
		new_heredoc_input = append_line_to_heredoc(heredoc_input,
				expanded_input);
		free(heredoc_input);
		heredoc_input = new_heredoc_input;
		write(fd, expanded_input, ft_strlen(expanded_input));
		write(fd, "\n", 1);
		free(input_buffer);
		free(expanded_input);
	}
	return (heredoc_input);
}

char	*handle_heredoc(t_token **current)
{
	char	*delimiter;
	char	*heredoc_input;
	int		fd;
	char	temp_file_name[30];
	int		temp_file_num;

	temp_file_num = 0;
	ft_strcpy(temp_file_name, "/tmp/minishell_heredoc");
	if (!current || !(*current))
		return (NULL);
	delimiter = (*current)->value;
	heredoc_input = NULL;
	fd = create_temp_file(temp_file_name, temp_file_num);
	if (fd == -1)
		return (NULL);
	heredoc_input = read_and_write_heredoc(fd, delimiter, heredoc_input);
	if (!heredoc_input)
	{
		close(fd);
		return (NULL);
	}
	close(fd);
	return (ft_strdup(temp_file_name));
}
