/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:43:15 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 04:46:20 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin(char **argv, t_env **env, int last_status)
{
	if (!argv || !argv[0])
		return (-1);
	if (ft_strcmp(argv[0], "echo") == 0)
		return (our_echo(argv));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (our_pwd());
	if (ft_strcmp(argv[0], "env") == 0)
		return (our_env(*env));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (our_unset(argv, env));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (our_cd(*env, argv));
	if (ft_strcmp(argv[0], "export") == 0)
		return (our_export(argv, env));
	if (ft_strcmp(argv[0], "exit") == 0)
		return (our_exit(argv, last_status));
	return (-1);
}
