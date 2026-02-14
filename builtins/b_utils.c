/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:43:15 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:35:35 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bfn	find_builtin(char *cmd)
{
	t_bentry	*table;
	int			i;

	if (!cmd)
		return (NULL);
	table = builtin_table();
	i = 0;
	while (table[i].name)
	{
		if (ft_strcmp(table[i].name, cmd) == 0)
			return (table[i].fn);
		i++;
	}
	return (NULL);
}

int	is_builtin_cmd(char *cmd)
{
	if (find_builtin(cmd))
		return (1);
	return (0);
}

int	builtin(char **argv, t_env **env, int last_status)
{
	t_bfn	fn;

	if (!argv || !argv[0])
		return (-1);
	fn = find_builtin(argv[0]);
	if (!fn)
		return (-1);
	return (fn(argv, env, last_status));
}
