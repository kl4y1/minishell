/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	b_cd(char **argv, t_env **env, int last_status)
{
	(void)last_status;
	return (our_cd(*env, argv));
}

int	b_export(char **argv, t_env **env, int last_status)
{
	(void)last_status;
	return (our_export(argv, env));
}

int	b_exit(char **argv, t_env **env, int last_status)
{
	(void)env;
	return (our_exit(argv, last_status));
}

void	printnonnumer(char *s)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}
