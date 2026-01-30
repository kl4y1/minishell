/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 21:54:26 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/19 18:59:02 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(char *big, char *little, int len)
{
	int	i;
	int	n;

	if (*little == '\0')
		return ((char *)big);
	n = ft_strlen(little);
	i = 0;
	while (big[i] && i + n <= len)
	{
		if (ft_strncmp(big + i, little, n) == 0)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
