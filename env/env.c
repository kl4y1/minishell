/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		env_add_front(&env, new);
		i++;
	}
	return (env);
}
