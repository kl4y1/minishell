/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:01 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:35:35 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_valid_ident(char *arg)
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

char	*export_key(char *arg)
{
	return (find_key(arg));
}

int	export_add_new(char *key, char *arg, t_env **env)
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
