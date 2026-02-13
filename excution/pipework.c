/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	paf_child_process(t_cmd *node, t_env **env, int w_fd, int ls)
{
	reset_signals();
	if (dup2(w_fd, STDOUT_FILENO) == -1)
		exit_if_error("dup2");
	close(w_fd);
	close_extra_fds();
	if (do_redirs(node->redirs) != 0)
		exit(1);
	exec_command(node->argv, env, ls);
}

pid_t	paf(t_cmd *node, t_env **env, int last_stat)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		exit_if_error("pipe");
	pid = fork();
	if (pid < 0)
		exit_if_error("fork");
	else if (pid == 0)
	{
		close(fd[0]);
		paf_child_process(node, env, fd[1], last_stat);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exit_if_error("dup2");
	close(fd[0]);
	return (pid);
}

pid_t	exec_final_command(t_cmd *node, t_env **env, int last_stat)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		exit_if_error("fork");
	if (pid == 0)
	{
		reset_signals();
		close_extra_fds();
		if (do_redirs(node->redirs) != 0)
			exit(1);
		exec_command(node->argv, env, last_stat);
	}
	return (pid);
}

int	pipeline(t_cmd *cmds, t_env **env, int *last_stat)
{
	t_pid	*pid_list;
	pid_t	last_pid;
	int		exitcode;

	pid_list = NULL;
	get_middle_cmds(cmds, env, &pid_list, *last_stat);
	last_pid = get_last_cmd(getg_last_cmd(cmds), env, &pid_list, *last_stat);
	exitcode = wait_pids(pid_list, last_pid, last_stat);
	free_pid_list(&pid_list);
	return (exitcode);
}
