/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:35:35 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_count(t_env *env)
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

void	sort_env_arr(t_env **arr, int n)
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

void	print_export(t_env *env)
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

int	export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

int	process_arg(char *arg, t_env **env)
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
