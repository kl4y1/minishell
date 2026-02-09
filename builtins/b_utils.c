/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:43:15 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 22:38:25 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//locked


static int	b_echo(char **argv, t_env **env, int last_status)
{
	(void)env;
	(void)last_status;
	return (our_echo(argv));
}

static int	b_pwd(char **argv, t_env **env, int last_status)
{
	(void)argv;
	(void)env;
	(void)last_status;
	return (our_pwd());
}

static int	b_env(char **argv, t_env **env, int last_status)
{
	(void)argv;
	(void)last_status;
	return (our_env(*env));
}

static int	b_unset(char **argv, t_env **env, int last_status)
{
	(void)last_status;
	return (our_unset(argv, env));
}

static int	b_cd(char **argv, t_env **env, int last_status)
{
	(void)last_status;
	return (our_cd(*env, argv));
}

static int	b_export(char **argv, t_env **env, int last_status)
{
	(void)last_status;
	return (our_export(argv, env));
}

static int	b_exit(char **argv, t_env **env, int last_status)
{
	(void)env;
	return (our_exit(argv, last_status));
}

static t_bentry	*builtin_table(void)
{
	static t_bentry	e[] = {
	{"echo", b_echo},
	{"pwd", b_pwd},
	{"env", b_env},
	{"unset", b_unset},
	{"cd", b_cd},
	{"export", b_export},
	{"exit", b_exit},
	{NULL, NULL}
	};

	return (e);
}

static t_bfn	find_builtin(char *cmd)
{
	t_bentry	*table;
	int			i;

	if (!cmd)
		return (NULL);
	table = builtin_table();
	i = 0;
	while (table[i].name)
	{
		if (ft_strcmp(table[i].name, cmd) == 0)
			return (table[i].fn);
		i++;
	}
	return (NULL);
}

int	is_builtin_cmd(char *cmd)
{
	if (find_builtin(cmd))
		return (1);
	return (0);
}

int	builtin(char **argv, t_env **env, int last_status)
{
	t_bfn	fn;

	if (!argv || !argv[0])
		return (-1);
	fn = find_builtin(argv[0]);
	if (!fn)
		return (-1);
	return (fn(argv, env, last_status));
}

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

void	printnonnumer(char *s)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(s, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}
