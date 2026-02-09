/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipework.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 21:03:02 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 20:10:55 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <errno.h>

static void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 256)
	{
		close(fd);
		fd++;
	}
}

static int	has_slash(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

static void	exec_not_found(char **cmd, char **paths, char **envp)
{
	if (has_slash(cmd[0]))
	{
		if (access(cmd[0], F_OK) == 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd[0]);
			freesplit(paths);
			free_arr(envp);
			exit(126);
		}
		ft_putstr_fd("minishell: ", 2);
		perror(cmd[0]);
		freesplit(paths);
		free_arr(envp);
		exit(127);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	freesplit(paths);
	free_arr(envp);
	exit(127);
}

static char	*load_exec_path(char **cmd, t_env **env, char ***paths, char ***envp)
{
	char	*exec_path;

	*envp = listtoarr(*env);
	if (!*envp)
		exit(1);
	*paths = get_paths(*envp);
	exec_path = find_exec(cmd[0], *paths);
	if (!exec_path)
		exec_not_found(cmd, *paths, *envp);
	return (exec_path);
}

static void	execve_fail(char *cmd0, char **envp, char *exec_path)
{
	ft_putstr_fd("minishell: ", 2);
	perror(cmd0);
	free_arr(envp);
	free(exec_path);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}

void	exec_command(char **cmd, t_env **env, int last_stat)
{
	char	**paths;
	char	*exec_path;
	char	**envp;
	int		ret;

	close_extra_fds();
	if (!cmd || !cmd[0])
		exit(0);
	ret = builtin(cmd, env, last_stat);
	if (ret != -1)
		exit(ret);
	exec_path = load_exec_path(cmd, env, &paths, &envp);
	freesplit(paths);
	if (execve(exec_path, cmd, envp) == -1)
		execve_fail(cmd[0], envp, exec_path);
	exit(1);
}

static void	paf_child_process(t_cmd *node, t_env **env, int w_fd, int ls)
{
	reset_signals();
	if (dup2(w_fd, STDOUT_FILENO) == -1)
		exit_if_error("dup2");
	close(w_fd);
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
