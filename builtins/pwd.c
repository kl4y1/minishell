/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:32:47 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/27 18:03:50 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//locked
int our_pwd(void)
{
    char *location;

    location = getcwd(NULL, 0);
    if (!location)
    {
        write(2,"Error getting current path",26);
        return (1);
    }
    ft_putstr_fd(location, 1);
    write(1, "\n", 1);
    free(location);
    return (0);
}
