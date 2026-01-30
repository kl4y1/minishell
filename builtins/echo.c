/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:32:14 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/27 19:03:12 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//locked

int our_echo(char **argv)
{
    int i;
    int flag;

    i = 1;
    flag = 0;
    while(checkflag(argv[i]))
        i++;
    if(i > 1)
        flag = 1;
    while(argv[i])
    {
        ft_putstr_fd(argv[i],1);
        if(argv[i+1])
            write(1," ",1);
        i++;
    }
    if(!flag)
        write(1,"\n",1);
    return 0;
}
