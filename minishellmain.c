/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellmain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 20:31:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:56:01 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_word(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (!zspace(s[i]))
			return (1);
		i++;
	}
	return (0);
}

static void	init_shell(char **envp, t_env **env, int *status)
{
	*env = envptoenv(envp);
	*status = 0;
	fresh_screen();
	prepare_sig();
}

void	handle_input(char *shell, int *last_stat)
{
	if (g_signal == SIGINT)
	{
		*last_stat = 130;
		g_signal = 0;
	}
	if (shell && shell[0])
		add_history(shell);
}

t_cmd	*parse_input(char *shell, t_env *env, int *last_stat)
{
	t_cmd	*cmds;

	cmds = parse_line(shell, env, *last_stat);
	if (!cmds && has_word(shell))
	{
		if (g_signal == SIGINT)
		{
			*last_stat = 130;
			g_signal = 0;
		}
		else
			*last_stat = 2;
	}
	return (cmds);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	t_env	*env;

	(void)argc;
	(void)argv;
	init_shell(envp, &env, &status);
	if (!env && envp && envp[0])
		return (1);
	main_loop(env, &status);
	rl_clear_history();
	env_free(env);
	return (status);
}
