/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafurnic <mafurnic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/05 11:24:36 by mafurnic          #+#    #+#             */
/*   Updated: 2024/04/10 17:15:04 by mafurnic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*#include <ctype.h>
#include <stdio.h>*/

int	ft_isalnum(int c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z' )
        || (c >= '0' && c <= '9') || (c == ' '))
        return (1);
    return (0);
}
/*int	main()
{
	char t = 'i';

	printf("%d\n",isalnum(t));
	printf("%d\n",ft_isalnum(t));
}*/
