/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 256)
	{
		close(fd);
		fd++;
	}
}

void	execve_fail(char *cmd0, char **envp, char *exec_path)
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

static void	exec_not_found(char **cmd, char **paths, char **envp, t_env *env)
{
	if (has_slash(cmd[0]))
	{
		if (access(cmd[0], F_OK) == 0)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(cmd[0]);
			freesplit(paths);
			free_arr(envp);
			env_free(env);
			exit(126);
		}
		ft_putstr_fd("minishell: ", 2);
		perror(cmd[0]);
		freesplit(paths);
		free_arr(envp);
		env_free(env);
		exit(127);
	}
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	freesplit(paths);
	free_arr(envp);
	env_free(env);
	exit(127);
}

char	*load_exec_path(char **cmd, t_env **env, char ***paths, char ***envp)
{
	char	*exec_path;

	*envp = listtoarr(*env);
	if (!*envp)
		exit(1);
	*paths = get_paths(*envp);
	exec_path = find_exec(cmd[0], *paths);
	if (!exec_path)
		exec_not_found(cmd, *paths, *envp, *env);
	return (exec_path);
}
