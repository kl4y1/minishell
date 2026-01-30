/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 21:04:18 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/19 18:59:02 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(void *s1, void *s2, int n)
{
	unsigned char	*s;
	unsigned char	*ss;

	s = s1;
	ss = s2;
	while (n--)
	{
		if (*s != *ss)
			return (*s - *ss);
		s++;
		ss++;
	}
	return (0);
}
// int main(void){
//     char * i = "hello";
//     char * j = "hr";
//     printf("%d\n",memcmp(i,j,3));
// }