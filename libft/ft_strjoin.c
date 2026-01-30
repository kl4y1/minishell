/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:58:07 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/23 16:58:43 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char *s1, char *s2)
{
	int		l1;
	int		l2;
	char	*ns;
	int		i;

	if (!s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	ns = (char *)malloc(l1 + l2 + 1);
	if (!ns)
		return (NULL);
	i = -1;
	while (++i < l1)
		ns[i] = s1[i];
	i = -1;
	while (++i < l2)
		ns[l1 + i] = s2[i];
	ns[l1 + l2] = '\0';
	return (ns);
}
