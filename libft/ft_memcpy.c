/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:20:22 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/19 18:59:02 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, void *src, int n)
{
	unsigned char	*ptr;
	unsigned char	*sc;
	int				i;

	sc = (unsigned char *)src;
	ptr = (unsigned char *)dest;
	i = 0;
	while (i < n)
	{
		ptr[i] = sc[i];
		i++;
	}
	return (dest);
}
