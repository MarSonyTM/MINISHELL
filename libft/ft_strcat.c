/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 12:48:18 by mafurnic          #+#    #+#             */
/*   Updated: 2024/02/26 12:49:25 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char *ft_strcat(char *dest, const char *src)
{
    char *ptr = dest;
    
    // Move pointer to the end of dest
    while (*ptr)
        ptr++;
    
    // Append src to dest
    while (*src)
    {
        *ptr = *src;
        ptr++;
        src++;
    }
    
    // Null-terminate the concatenated string
    *ptr = '\0';
    
    return dest;
}
