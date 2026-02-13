/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:38 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/12 05:18:04 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	is_numeric(char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '-' || s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(char *s)
{
	int			i;
	int			neg;
	long long	val;

	i = 0;
	neg = 0;
	val = 0;
	if (s[i] == '-' || s[i] == '+')
		neg = (s[i++] == '-');
	while (s[i])
	{
		if (val > (LLONG_MAX - (s[i] - '0')) / 10)
			return (1);
		val = val * 10 + (s[i] - '0');
		i++;
	}
	if (neg && (unsigned long long)val > (unsigned long long)LLONG_MAX + 1ULL)
		return (1);
	return (0);
}

static long long	ft_atoll(char *s)
{
	int			i;
	int			neg;
	long long	val;

	i = 0;
	neg = 0;
	val = 0;
	if (s[i] == '-' || s[i] == '+')
		neg = (s[i++] == '-');
	while (s[i])
		val = val * 10 + (s[i++] - '0');
	if (neg)
		return (-val);
	return (val);
}

int	our_exit(char **argv, int last_status)
{
	ft_putstr_fd("exit\n", 2);
	if (!argv[1])
		return (256 + last_status);
	if (!is_numeric(argv[1]) || check_overflow(argv[1]))
	{
		printnonnumer(argv[1]);
		return (256 + 2);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	return (256 + (unsigned char)ft_atoll(argv[1]));
}
