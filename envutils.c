/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envutils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 01:46:36 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 00:45:01 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//needs checking except for new node


void	env_add_front(t_env **env, t_env *new)
{
	if (!new)
		return ;
	new->next = *env;
	*env = new;
}

void	env_free(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	env_free_one(t_env *n)
{
	if (!n)
		return ;
	free(n->key);
	free(n->value);
	free(n);
}


t_env	*env_new_node(char *s)
{
	char	*idx;
	t_env	*n;

	n = malloc(sizeof(t_env));
	if (!n)
		return (NULL);
	idx = ft_strchr(s, '=');
	if (idx)
	{
		n->key = ft_substr(s, 0, idx - s);
		n->value = ft_strdup(idx + 1);
	}
	else
	{
		n->key = ft_strdup(s);
		n->value = ft_strdup("");
	}
	if (!n->key || !n->value)
	{
		env_free_one(n);
		return (NULL);
	}
	n->next = NULL;
	return (n);
}

t_env	*envptoenv(char **envp)
{
	t_env	*env;
	t_env	*new;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new = env_new_node(envp[i]);
		if (!new)
		{
			env_free(env);
			return (NULL);
		}
		env_add_front(&env,new);
		i++;
	}
	return (env);
}

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

    arr = malloc(sizeof(char *) * (env_size(env) + 1));
    if (!arr)
        return (NULL);
    i = 0;
    while (env)
    {
        arr[i] = env_to_str(env);
        if (!arr[i])
            return (free_arr(arr), NULL);
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
