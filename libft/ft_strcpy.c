/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturm <csturm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 12:50:26 by mafurnic          #+#    #+#             */
/*   Updated: 2024/03/12 17:35:48 by csturm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    
    // Copy characters from src to dest
    while (*src)
    {
        *ptr = *src;
        ptr++;
        src++;
    }
    
    // Null-terminate the destination string
    *ptr = '\0';
    
    return dest;
}
