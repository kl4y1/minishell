/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 22:12:38 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/19 18:59:02 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(int nmemb, int size)
{
	int		fsz;
	void	*p;

	if (nmemb == 0 || size == 0)
	{
		p = malloc(1);
		if (!p)
			return (NULL);
		return (p);
	}
	fsz = nmemb * size;
	p = malloc(fsz);
	if (!p)
		return (NULL);
	ft_bzero(p, fsz);
	return (p);
}
