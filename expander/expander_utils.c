/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 23:56:01 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 22:41:24 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	varname_len(char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	if (s[0] == '?')
		return (1);
	i = 0;
	while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
		i++;
	return (i);
}

char	*get_var_val(char *name, t_env *env, int last_stat)
{
	t_env	*node;

	if (!name || !name[0])
		return (ft_strdup("$"));
	if (name[0] == '?')
		return (ft_itoa(last_stat));
	node = env_find(env, name);
	if (node && node->value)
		return (ft_strdup(node->value));
	return (ft_strdup(""));
}

char	*append_str(char *res, char *add)
{
	char	*tmp;

	tmp = ft_strjoin(res, add);
	free(res);
	return (tmp);
}

char	*append_char(char *res, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (append_str(res, buf));
}

t_token	*tok_tail(t_token *node)
{
	if (!node)
		return (NULL);
	while (node->next)
		node = node->next;
	return (node);
}


