/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipework.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 21:03:02 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 23:32:04 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_command(char **cmd, t_env **env)
{
	char	**paths;
	char	*exec_path;
	char	**envp;
	int		ret;

	ret = builtin(cmd, env, 0);
	if (ret != -1)
		exit(ret);
	envp = listtoarr(*env);
	paths = get_paths(envp);
	if (!paths)
	{
		free_arr(envp);
		exit(1);
	}
	exec_path = find_exec(cmd[0], paths);
	if (!exec_path)
		exec_path_error(cmd, paths);
	freesplit(paths);
	if (execve(exec_path, cmd, envp) == -1)
		error_execve(cmd, exec_path);
	exit(1);
}

void	paf_child_process(t_cmd *node, t_env **env, int w_fd)
{
	reset_signals();
	if (dup2(w_fd, STDOUT_FILENO) == -1)
		exit_if_error("dup2");
	close(w_fd);
	if (do_redirs(node->redirs) != 0)
		exit(1);
	exec_command(node->argv, env);
}

pid_t	paf(t_cmd *node, t_env **env)
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
		paf_child_process(node, env, fd[1]);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exit_if_error("dup2");
	close(fd[0]);
	return (pid);
}

pid_t	exec_final_command(t_cmd *node, t_env **env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		exit_if_error("fork");
	if (pid == 0)
	{
		reset_signals();
		if (do_redirs(node->redirs) != 0)
			exit(1);
		exec_command(node->argv, env);
	}
	return (pid);
}

int	pipeline(t_cmd *cmds, t_env **env, int *last_stat)
{
	t_pid	*pid_list;
	pid_t	last_pid;
	int		exitcode;

	pid_list = NULL;
	get_middle_cmds(cmds, env, &pid_list);
	last_pid = get_last_cmd(getg_last_cmd(cmds), env, &pid_list);
	exitcode = wait_pids(pid_list, last_pid, last_stat);
	free_pid_list(&pid_list);
	return (exitcode);
}
