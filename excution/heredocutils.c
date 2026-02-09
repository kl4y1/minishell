/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocutils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 05:53:27 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 05:55:08 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	hd_sigint(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	write(1, "\n", 1);
	rl_done = 1;
}

void	hd_set_signals(struct sigaction *old_int, struct sigaction *old_quit)
{
	struct sigaction	sa;

	sigaction(SIGINT, NULL, old_int);
	sigaction(SIGQUIT, NULL, old_quit);
	sa.sa_handler = hd_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	hd_restore_signals(struct sigaction *old_int,
		struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}

void	hd_cleanup(t_hd *hd)
{
	close(hd->fd);
	free(hd->clean);
	if (hd->res)
	{
		unlink(hd->path);
		free(hd->path);
		hd->path = NULL;
	}
}
