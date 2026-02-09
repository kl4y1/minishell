/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellmain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 20:31:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 20:10:56 by mnajem           ###   ########.fr       */
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

int	main(int argc, char **argv, char **envp)
{
	char	*shell;
	t_cmd	*cmds;
	int		status;
	t_env	*env;
	int		last_stat;
	int		save_in;
	int		save_out;
	int		ret;

	(void)argc;
	(void)argv;
	last_stat = 0;
	env = envptoenv(envp);
	if (!env && envp && envp[0])
		return (1);
	status = 0;
	fresh_screen();
	prepare_sig();
	while (1)
	{
		shell = readline(C_PURPLE "alo?$ " C_RESET);
		if (g_signal == SIGINT)
		{
			last_stat = 130;
			g_signal = 0;
		}
		if (!shell)
		{
			printf("exit\n");
			break ;
		}
		cmds = NULL;
		if (shell[0])
		{
			add_history(shell);
			cmds = parse_line(shell, env, last_stat);
			if (!cmds && has_word(shell))
			{
				if (g_signal == SIGINT)
				{
					last_stat = 130;
					g_signal = 0;
				}
				else
					last_stat = 2;
			}
		}
		if (cmds)
		{
			save_in = dup(STDIN_FILENO);
			save_out = dup(STDOUT_FILENO);
			if (save_in < 0 || save_out < 0)
			{
				perror("dup");
				if (save_in >= 0)
					close(save_in);
				if (save_out >= 0)
					close(save_out);
				free_cmds(cmds);
				free(shell);
				status = 1;
				last_stat = 1;
				continue ;
			}
			fcntl(save_in, F_SETFD, FD_CLOEXEC);
			fcntl(save_out, F_SETFD, FD_CLOEXEC);
			ret = -1;
			if (!cmds->next)
			{
				if (!cmds->argv || !cmds->argv[0])
				{
					if (do_redirs(cmds->redirs) == 0)
						ret = 0;
					else
						ret = 1;
				}
				else if (is_builtin_cmd(cmds->argv[0]))
				{
					if (do_redirs(cmds->redirs) == 0)
						ret = builtin(cmds->argv, &env, last_stat);
					else
						ret = 1;
				}
			}
			if (ret == -1)
				status = pipeline(cmds, &env, &last_stat);
			else
			{
				status = ret;
				last_stat = ret;
			}
			if (dup2(save_in, STDIN_FILENO) < 0)
				perror("dup2");
			close(save_in);
			if (dup2(save_out, STDOUT_FILENO) < 0)
				perror("dup2");
			close(save_out);
			if (status >= 256)
			{
				status = status - 256;
				free_cmds(cmds);
				free(shell);
				break ;
			}
		}
		free_cmds(cmds);
		free(shell);
	}
	rl_clear_history();
	env_free(env);
	return (status);
}
