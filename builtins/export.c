/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:01 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 20:46:05 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//locked
static int	export_valid_ident(char *arg)
{
	int	i;

	if (!arg || !arg[0])
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	if (arg[i] == '=' || arg[i] == '\0')
		return (1);
	return (0);
}

static char	*export_key(char *arg)
{
	return (find_key(arg));
}

static int	export_add_new(char *key, char *arg, t_env **env)
{
	t_env	*node;
	char	*idx;

	node = malloc(sizeof(t_env));
	if (!node)
		return (1);
	node->key = ft_strdup(key);
	idx = ft_strchr(arg, '=');
	if (idx)
		node->value = ft_strdup(idx + 1);
	else
		node->value = NULL;
	if (!node->key || (idx && !node->value))
	{
		env_free_one(node);
		return (1);
	}
	node->next = NULL;
	env_add_back(env, node);
	return (0);
}

static int	env_count(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

static void	sort_env_arr(t_env **arr, int n)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < n - 1)
	{
		j = i + 1;
		while (j < n)
		{
			if (ft_strcmp(arr[i]->key, arr[j]->key) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

static void	print_export(t_env *env)
{
	t_env	**arr;
	int		n;
	int		i;

	n = env_count(env);
	arr = malloc(sizeof(t_env *) * n);
	if (!arr)
		return ;
	i = 0;
	while (env)
	{
		arr[i++] = env;
		env = env->next;
	}
	sort_env_arr(arr, n);
	i = 0;
	while (i < n)
	{
		if (arr[i]->value)
			printf("declare -x %s=\"%s\"\n", arr[i]->key, arr[i]->value);
		else
			printf("declare -x %s\n", arr[i]->key);
		i++;
	}
	free(arr);
}

static int	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static int	process_arg(char *arg, t_env **env)
{
	t_env	*existing;
	char	*key;

	key = export_key(arg);
	if (!key)
		return (1);
	existing = env_find(*env, key);
	if (existing)
		update_env(existing, arg);
	else
	{
		if (export_add_new(key, arg, env))
		{
			free(key);
			return (1);
		}
	}
	free(key);
	return (0);
}

int	our_export(char **argv, t_env **env)
{
	int	i;
	int	ret;

	if (!argv[1])
	{
		print_export(*env);
		return (0);
	}
	i = 0;
	ret = 0;
	while (argv[++i])
	{
		if (!export_valid_ident(argv[i]))
			ret = export_error(argv[i]);
		else if (process_arg(argv[i], env))
			ret = 1;
	}
	return (ret);
}
