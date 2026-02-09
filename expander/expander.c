/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 23:58:19 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 22:55:44 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
//dont even try to understand this shit zaza
static char	*handle_var(char *s, char *res, t_exp *e)
{
	int		vlen;
	char	*vname;
	char	*vval;

	e->i++;
	if (!s[e->i] || s[e->i] == 34 || s[e->i] == 39 || zspace(s[e->i]))
		return (append_char(res, '$'));
	vlen = varname_len(s + e->i);
	if (vlen == 0)
		return (append_char(res, '$'));
	vname = ft_substr(s, e->i, vlen);
	if (!vname)
		return (res);
	vval = get_var_val(vname, e->env, e->last_stat);
	free(vname);
	if (vval && e->quote == 0)
		res = exp_append_splitable(res, vval);
	else if (vval)
		res = append_str(res, vval);
	free(vval);
	e->i += vlen;
	return (res);
}

static char	*process_char(char *s, char *res, t_exp *e, int *had_quotes)
{
	char	*tmp;

	tmp = exp_handle_backslash(s, res, e);
	if (tmp)
		return (tmp);
	if (e->quote == 0 && (s[e->i] == 34 || s[e->i] == 39))
	{
		e->quote = s[e->i++];
		*had_quotes = 1;
		return (res);
	}
	if (e->quote == s[e->i])
	{
		e->quote = 0;
		e->i++;
		*had_quotes = 1;
		return (res);
	}
	if (s[e->i] == '$' && e->quote != 39)
		return (handle_var(s, res, e));
	return (append_char(res, s[e->i++]));
}

char	*do_expand(char *s, t_env *env, int last_stat, int *had_quotes)
{
	char	*res;
	t_exp	e;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	e.env = env;
	e.last_stat = last_stat;
	e.i = 0;
	e.quote = 0;
	*had_quotes = 0;
	while (s[e.i] && res)
		res = process_char(s, res, &e, had_quotes);
	return (res);
}

int	expand_tokens(t_token **tokens, t_env *env, int last_stat)
{
	t_tok	x;
	int		res;

	if (!tokens || !*tokens)
		return (0);
	tok_init(&x, tokens, env, last_stat);
	while (x.cur)
	{
		if (x.cur->type == WORD && x.cur->value)
		{
			res = tok_step(&x);
			if (res == -1)
				return (1);
			if (res == 2 || x.cur == NULL)
				continue ;
		}
		x.prev = x.cur;
		x.cur = x.cur->next;
	}
	return (0);
}
