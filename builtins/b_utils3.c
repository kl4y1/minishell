/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:40:59 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	b_echo(char **argv, t_env **env, int last_status)
{
	(void)env;
	(void)last_status;
	return (our_echo(argv));
}

static int	b_pwd(char **argv, t_env **env, int last_status)
{
	(void)argv;
	(void)env;
	(void)last_status;
	return (our_pwd());
}

static int	b_env(char **argv, t_env **env, int last_status)
{
	(void)argv;
	(void)last_status;
	return (our_env(*env));
}

static int	b_unset(char **argv, t_env **env, int last_status)
{
	(void)last_status;
	return (our_unset(argv, env));
}

t_bentry	*builtin_table(void)
{
	static t_bentry	e[] = {
	{"echo", b_echo},
	{"pwd", b_pwd},
	{"env", b_env},
	{"unset", b_unset},
	{"cd", b_cd},
	{"export", b_export},
	{"exit", b_exit},
	{NULL, NULL}
	};

	return (e);
}
