/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strok.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:50:51 by mafurnic          #+#    #+#             */
/*   Updated: 2024/02/26 12:49:11 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
}
