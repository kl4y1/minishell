/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:32:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 20:45:44 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//locked
static void	set_env_value(t_env *env, char *key, char *val)
{
	t_env	*node;
	char	*dup;

	node = env_find(env, key);
	if (!node || !val)
		return ;
	dup = ft_strdup(val);
	if (!dup)
		return ;
	free(node->value);
	node->value = dup;
}

static char	*resolve_cd_path(t_env *env, char **argv, int *print_pwd)
{
	t_env	*key;

	*print_pwd = 0;
	if (!argv[1])
	{
		key = env_find(env, "HOME");
		if (!key || !key->value || !key->value[0])
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
		return (key->value);
	}
	if (ft_strcmp(argv[1], "-") == 0)
	{
		key = env_find(env, "OLDPWD");
		if (!key || !key->value || !key->value[0])
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		*print_pwd = 1;
		return (key->value);
	}
	return (argv[1]);
}

static int	finish_cd(t_env *env, char *oldpwd, int print_pwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	set_env_value(env, "OLDPWD", oldpwd);
	set_env_value(env, "PWD", newpwd);
	if (print_pwd && newpwd)
	{
		ft_putstr_fd(newpwd, 1);
		write(1, "\n", 1);
	}
	free(oldpwd);
	free(newpwd);
	return (0);
}

int	count_args(char **av)
{
	int	i;

	i = 0;
	if (!av || !av[0])
		return (1);
	while (av[i])
		i++;
	if (i > 2)
		return (1);
	return (0);
}

int	our_cd(t_env *env, char **argv)
{
	char	*path;
	char	*oldpwd;
	int		print_pwd;

	if (count_args(argv))
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	oldpwd = getcwd(NULL, 0);
	path = resolve_cd_path(env, argv, &print_pwd);
	if (!path)
	{
		free(oldpwd);
		return (1);
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		free(oldpwd);
		return (1);
	}
	return (finish_cd(env, oldpwd, print_pwd));
}
