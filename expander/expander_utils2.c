/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 23:57:13 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/13 03:02:31 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*exp_append_splitable(char *res, char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == ' ' || s[i] == '\t')
			res = append_char(res, 0x1f);
		else
			res = append_char(res, s[i]);
		if (!res)
			return (NULL);
		i++;
	}
	return (res);
}

char	*exp_handle_backslash(char *s, char *res, t_exp *e)
{
	if (s[e->i] != '\\' || e->quote == 39)
		return (NULL);
	if (!s[e->i + 1])
		return (append_char(res, s[e->i++]));
	if (e->quote == 34 && (s[e->i + 1] == '$' || s[e->i + 1] == '"'
			|| s[e->i + 1] == '\\' || s[e->i + 1] == '\n'))
	{
		res = append_char(res, s[e->i + 1]);
		e->i += 2;
		return (res);
	}
	if (e->quote == 34)
		return (append_char(res, s[e->i++]));
	res = append_char(res, s[e->i + 1]);
	e->i += 2;
	return (res);
}

void	tok_init(t_tok *x, t_token **tokens, t_env *env, int last_stat)
{
	x->tokens = tokens;
	x->prev = NULL;
	x->cur = *tokens;
	x->env = env;
	x->last_stat = last_stat;
}
