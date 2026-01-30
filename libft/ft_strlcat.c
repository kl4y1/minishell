/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 15:09:44 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/19 18:59:02 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlcat(char *dst, char *src, int size)
{
	int	dst_len;
	int	src_len;
	int	i;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	if (size <= dst_len)
	{
		return (size + src_len);
	}
	i = dst_len;
	while (i < size - 1 && *src != '\0')
	{
		dst[i] = *src;
		i++;
		src++;
	}
	dst[i] = '\0';
	return (dst_len + src_len);
}

// int	main(void)
// {
// 	char dst[20] = "alo";
// 	char src[20] = "yaali";
// 	int size = 3;
// 	printf("%zu\n", ft_strlcat(dst, src, size));
// }
