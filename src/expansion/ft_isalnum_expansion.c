/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum_expansion.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marianfurnica <marianfurnica@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 17:55:51 by marianfurni       #+#    #+#             */
/*   Updated: 2024/04/19 17:56:12 by marianfurni      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_isalnum_expansion(int c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z' )
        || (c >= '0' && c <= '9'))
        return (1);
    return (0);
}