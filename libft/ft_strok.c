/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strok.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 14:24:27 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/16 14:36:22 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*get_next_token(char *str, const char *delim)
{
	int		i;
	int		j;
	char	*token;

	i = 0;
	while (str[i] != '\0' && ft_strchr(delim, str[i]) != NULL)
		i++;
	token = &str[i];
	j = i;
	while (str[j] != '\0' && ft_strchr(delim, str[j]) == NULL)
		j++;
	if (str[j] != '\0')
	{
		str[j] = '\0';
		return (&str[j + 1]);
	}
	return (NULL);
}

char	*ft_strtok(char *str, const char *delim)
{
	static char	*last;
	char		*token;

	if (str == NULL)
	{
		str = last;
		if (str == NULL)
		{
			return (NULL);
		}
	}
	last = get_next_token(str, delim);
	token = str;
	if (last == NULL && str[0] == '\0')
		return (NULL);
	return (token);
}
