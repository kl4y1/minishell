/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:32:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 19:33:57 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int count_args(char **av)
{
    int i;

    i = 0;
    if(!av || !av[0])
        return 1;
    while(av[i])
        i++;
    if(i > 2)
        return 1;
    return 0;
}

int	our_cd(t_env *env, char **argv)
{
	(void)env;
	if (count_args(argv))
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!argv[1])
		return (0);
	if (chdir(argv[1]) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(argv[1]);
		return (1);
	}
	return (0);
}