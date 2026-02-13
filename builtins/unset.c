/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:13 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/11 21:57:10 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// locked
int	valid_ident(char *s)
{
	int	i;

	i = 1;
	if (!s || !s[0])
		return (0);
	if (ft_isalpha(s[0]) || s[0] == '_')
	{
		while (s[i])
		{
			if (!ft_isalnum(s[i]) && s[i] != '_')
				return (0);
			i++;
		}
	}
	else
		return (0);
	return (1);
}

void	unset_key(t_env **env, char *key)
{
	t_env	*temp;
	t_env	*to_free;

	temp = NULL;
	if (!env || !*env || !key)
		return ;
	while (*env)
	{
		if ((*env)->key && ft_strcmp((*env)->key, key) == 0)
		{
			to_free = *env;
			if (temp)
				temp->next = (*env)->next;
			else
				*env = (*env)->next;
			free(to_free->key);
			free(to_free->value);
			free(to_free);
			return ;
		}
		temp = *env;
		env = &((*env)->next);
	}
}

int	our_unset(char **argv, t_env **env)
{
	int	i;
	int	alo;

	alo = 0;
	i = 0;
	if (ft_strcmp(argv[0], "unset") == 0)
	{
		i = 1;
		while (argv[i])
		{
			if (!valid_ident(argv[i]))
			{
				ft_putstr_fd("minishell: unset: `", 2);
				ft_putstr_fd(argv[i], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
				alo = 1;
			}
			else
				unset_key(env, argv[i]);
			i++;
		}
	}
	return (alo);
}
