/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:32:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/01 22:44:39 by mnajem           ###   ########.fr       */
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

int our_cd(t_env env,char **argv)
{
    char *newlo;
    if(count_args(argv))
    {
        perror("TOO MANY ARGS FOR CD");
        return 0;
    }
    
    
    
}