/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipework.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 21:03:02 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/30 16:43:25 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_command(char **cmd, char **envp)
{
	char	**paths;
	char	*exec_path;

	paths = get_paths(envp);
	if (!paths)
	{
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

void	paf_child_process(t_cmd *node, char **envp, int w_fd)
{
    
	if (dup2(w_fd, STDOUT_FILENO) == -1)
		exit_if_error("dup2");
	close(w_fd);
    apply_redirs(node->redirs);
	exec_command(node->argv, envp);
}

pid_t	paf(t_cmd *node, char **envp)
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
		paf_child_process(node, envp, fd[1]);
	}
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		exit_if_error("dup2");
	close(fd[0]);
    
    return pid;
}

pid_t	exec_final_command(t_cmd *node, char **envp)
{
	pid_t	pid;
    
	pid = fork();
	if (pid < 0)
		exit_if_error("fork");
	if (pid == 0)
    {
        apply_redirs(node->redirs);
		exec_command(node->argv, envp);
    }
    return pid;
}
//make it t_cmd when you build the env
int	pipeline(t_cmd *cmds, char **env)
{
	t_pid	*pid_list;
	pid_t	last_pid;
	int		exitcode;

	pid_list = NULL;
	get_middle_cmds(cmds, env, &pid_list);
	last_pid = get_last_cmd(getg_last_cmd(cmds), env, &pid_list);
	exitcode = wait_pids(pid_list, last_pid);
	free_pid_list(&pid_list);
	return (exitcode);
}

