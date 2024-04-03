/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 13:49:41 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/03 13:49:49 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char *handle_heredoc(t_token **current)
{
    if (!current || !(*current)) return NULL;

    char *delimiter = (*current)->value;
    char *heredoc_input = NULL;
    char *input_buffer;

    while (1)
    {
        ft_putstr_fd("> ", 1);
        input_buffer = readline(NULL);
        if (!input_buffer) return (NULL);
        if (ft_strcmp(input_buffer, delimiter) == 0)
        {
            free(input_buffer);
            break;
        }
        char *temp;
        if (heredoc_input) {
            temp = ft_strjoin(heredoc_input, "\n");
        } else {
            temp = ft_strdup("");
        }
        free(heredoc_input);
        heredoc_input = ft_strjoin(temp, input_buffer);
        free(temp);
        free(input_buffer);
    }
    return (heredoc_input);
}