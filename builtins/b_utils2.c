/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:35:35 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_key(char *s)
{
	char	*k;

	k = ft_strchr(s, '=');
	if (k)
		return (ft_substr(s, 0, k - s));
	return (ft_strdup(s));
}

t_env	*env_find(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	env_add_back(t_env **env, t_env *new)
{
	t_env	*cur;

	if (!env || !new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	cur = *env;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

t_env	*new_node(char *s)
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
		n->value = NULL;
	}
	if ((idx && (!n->key || !n->value)) || (!idx && !n->key))
	{
		env_free_one(n);
		return (NULL);
	}
	n->next = NULL;
	return (n);
}

void	update_env(t_env *existing, char *arg)
{
	char	*idx;
	char	*newv;

	idx = ft_strchr(arg, '=');
	if (!idx)
		return ;
	newv = ft_strdup(idx + 1);
	if (!newv)
		return ;
	free(existing->value);
	existing->value = newv;
}
