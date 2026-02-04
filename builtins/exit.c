/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:38 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 19:33:42 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//needs fixing , overflow
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

int	our_exit(char **argv, int last_status)
{
    ft_putstr_fd("exit\n", 2);
    if (!argv[1])
        exit(last_status);
    if (!is_numeric(argv[1]))
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(argv[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(255);
    }
    if (argv[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        return (1);
    }
    exit((unsigned char)ft_atoi(argv[1]));
}
