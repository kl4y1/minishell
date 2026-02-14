/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellmain2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_dup_error(int save_in, int save_out, t_cmd *cmds,
	char *shell)
{
	perror("dup");
	if (save_in >= 0)
		close(save_in);
	if (save_out >= 0)
		close(save_out);
	free_cmds(cmds);
	free(shell);
	return (1);
}

static int	handle_single_cmd(t_cmd *cmds, t_env **env, int last_stat)
{
	int	ret;

	ret = -1;
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
			ret = builtin(cmds->argv, env, last_stat);
		else
			ret = 1;
	}
	return (ret);
}

void	restore_fds(int save_in, int save_out)
{
	if (dup2(save_in, STDIN_FILENO) < 0)
		perror("dup2");
	close(save_in);
	if (dup2(save_out, STDOUT_FILENO) < 0)
		perror("dup2");
	close(save_out);
}

int	exec_cmds(t_cmd *cmds, t_env **env, int *last_stat)
{
	int	save_in;
	int	save_out;
	int	ret;
	int	status;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (save_in < 0 || save_out < 0)
		return (handle_dup_error(save_in, save_out, cmds, NULL));
	fcntl(save_in, F_SETFD, FD_CLOEXEC);
	fcntl(save_out, F_SETFD, FD_CLOEXEC);
	ret = -1;
	if (!cmds->next)
		ret = handle_single_cmd(cmds, env, *last_stat);
	if (ret == -1)
		status = pipeline(cmds, env, last_stat);
	else
	{
		status = ret;
		*last_stat = ret;
	}
	restore_fds(save_in, save_out);
	return (status);
}
