/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellmain3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_command(char *shell, t_env **env, int *last_stat)
{
	t_cmd	*cmds;
	int		status;

	cmds = parse_input(shell, *env, last_stat);
	if (cmds)
	{
		status = exec_cmds(cmds, env, last_stat);
		free_cmds(cmds);
		return (status);
	}
	return (0);
}

int	main_loop(t_env *env, int *status)
{
	char	*shell;
	int		last_stat;
	int		ret;

	last_stat = 0;
	while (1)
	{
		shell = readline(C_PURPLE "alo?$ " C_RESET);
		handle_input(shell, &last_stat);
		if (!shell)
		{
			printf("exit\n");
			break ;
		}
		ret = process_command(shell, &env, &last_stat);
		if (ret >= 256)
		{
			*status = ret - 256;
			free(shell);
			return (1);
		}
		*status = ret;
		free(shell);
	}
	return (0);
}
