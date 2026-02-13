/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_size(t_env *env)
{
	int	size;

	size = 0;
	while (env)
	{
		size++;
		env = env->next;
	}
	return (size);
}

static char	*env_to_str(t_env *node)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(node->key, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, node->value);
	free(tmp);
	return (result);
}

char	**listtoarr(t_env *env)
{
	char	**arr;
	int		i;

	arr = ft_calloc(env_size(env) + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		arr[i] = env_to_str(env);
		if (!arr[i])
		{
			free_arr(arr);
			return (NULL);
		}
		i++;
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	free_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
