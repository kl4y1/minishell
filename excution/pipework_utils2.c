/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	get_last_cmd(t_cmd *cmd, t_env **env, t_pid **pid_list, int last_stat)
{
	pid_t	pid;

	pid = exec_final_command(cmd, env, last_stat);
	pid_add_back(pid_list, pid_node(pid));
	return (pid);
}

void	get_middle_cmds(t_cmd *cmds, t_env **env, t_pid **pid_list,
		int last_stat)
{
	t_cmd	*cur;
	pid_t	pid;

	cur = cmds;
	while (cur && cur->next)
	{
		pid = paf(cur, env, last_stat);
		pid_add_back(pid_list, pid_node(pid));
		cur = cur->next;
	}
}

static int	handle_wait_status(int status, pid_t cur_pid, pid_t last_pid,
		int *last_stat)
{
	int	exit_code;

	exit_code = 0;
	if (cur_pid == last_pid)
	{
		if (WIFEXITED(status))
			exit_code = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			exit_code = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGQUIT)
				write(2, "Quit: 3\n", 8);
			else if (WTERMSIG(status) == SIGINT)
				write(2, "\n", 1);
		}
		*last_stat = exit_code;
	}
	return (exit_code);
}

int	wait_pids(t_pid *pid_list, pid_t last_pid, int *last_stat)
{
	int		status;
	int		exit_code;
	t_pid	*cur;
	void	(*old_int)(int);
	void	(*old_quit)(int);

	exit_code = 0;
	old_int = signal(SIGINT, SIG_IGN);
	old_quit = signal(SIGQUIT, SIG_IGN);
	cur = pid_list;
	while (cur)
	{
		if (waitpid(cur->pid, &status, 0) != -1)
			exit_code = handle_wait_status(status, cur->pid, last_pid,
					last_stat);
		else
			perror("waitpid");
		cur = cur->next;
	}
	signal(SIGINT, old_int);
	signal(SIGQUIT, old_quit);
	return (exit_code);
}
