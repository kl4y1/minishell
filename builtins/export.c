/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:01 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/05 02:07:49 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_find(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	env_add_back(t_env **env, t_env *new)
{
	t_env *cur;

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

int valid_ident(char *s)
{
    int i;

    i = 1;
    if(!s || !s[0])
        return 0;
    if(ft_isalpha(s[0]) || s[0] == '_')
    {
        while(s[i])
        {
            if(!ft_isalnum(s[i]) && s[i] != '_')
                return 0;
            i++;
        }
    }
    else
        return 0;
    return 1;
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

int	our_export(char **argv, t_env **env)
{
    t_env *head;
    t_env *node;
    int i;

    i = 1;
    if(!argv[0]||!**argv)
    {
        return (1);
    }
    head = *env;
    if(!argv[1])
    {
	    while(head)
        {
            printf("declare -x %s=\"%s\"\n", head->key, head->value);
            head = head->next;
        }
    }
    else
    {
        while(argv[i])
        {
            if(valid_ident(argv[i]))
            {
                node = new_node(argv[i]);
                if(!node)
                    env_free(node);
                env_add_back(env,node);
                i++;
            }
        }
    }
	return (0);
}

