/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:26 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 20:50:24 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// locked
int	our_env(t_env *env)
{
	while (env)
	{
		if (env->key && env->value)
		{
			ft_putstr_fd(env->key, 1);
			write(1, "=", 1);
			ft_putstr_fd(env->value, 1);
			write(1, "\n", 1);
		}
		env = env->next;
	}
	return (0);
}
