/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strok.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:50:51 by mafurnic          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/26 12:49:11 by mafurnic         ###   ########.fr       */
=======
/*   Updated: 2024/02/21 12:06:35 by mafurnic         ###   ########.fr       */
>>>>>>> master
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

<<<<<<< HEAD
char *ft_strtok(char *str, const char *delim) {
    static char *last = NULL;
    char *token;
    int i, j;

    if (str == NULL && (str = last) == NULL)
        return NULL;

    i = 0;
    while (str[i] != '\0' && ft_strchr(delim, str[i]) != NULL)
        i++;
    
    if (str[i] == '\0') {
        last = NULL;
        return NULL;
    }

    token = &str[i];
    j = i;
    while (str[j] != '\0' && ft_strchr(delim, str[j]) == NULL)
        j++;
    
    if (str[j] != '\0') {
        str[j] = '\0';
        last = &str[j + 1];
    } else {
        last = NULL;
    }

    return token;
=======
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
>>>>>>> master
}
