/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strok.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:50:51 by mafurnic          #+#    #+#             */
/*   Updated: 2024/02/21 12:06:35 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*skip_delims(char *str, const char *delim)
{
	while (*str != '\0' && ft_strchr(delim, *str) != NULL)
		str++;
	return (str);
}

static char	*terminate_token(char *str, const char *delim, char **last_input)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_strchr(delim, str[i]) != NULL)
		{
			str[i] = '\0';
			*last_input = &str[i + 1];
			return (str);
		}
		i++;
	}
	*last_input = NULL;
	return (str);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last_input;
	char		*token;

	if (!str)
		str = last_input;
	if (!str)
		return (NULL);
	str = skip_delims(str, delim);
	if (*str == '\0')
		return (last_input = NULL);
	token = str;
	token = terminate_token(str, delim, &last_input);
	return (token);
}
