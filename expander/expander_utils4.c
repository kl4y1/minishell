/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tok_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 23:57:26 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 22:35:37 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	tok_load_words(t_tok *x)
{
	char	*expanded;

	expanded = do_expand(x->cur->value, x->env, x->last_stat, &x->had_quotes);
	if (!expanded)
		return (1);
	x->words = ft_split(expanded, 0x1f);
	free(expanded);
	if (!x->words)
		return (1);
	x->wcount = 0;
	while (x->words[x->wcount])
		x->wcount++;
	return (0);
}

int	tok_handle_zero(t_tok *x)
{
	t_token	*next;
	int		res;

	if (x->had_quotes)
	{
		res = tok_set_empty(x);
		freesplit(x->words);
		return (res);
	}
	next = x->cur->next;
	free(x->cur->value);
	free(x->cur);
	if (x->prev)
		x->prev->next = next;
	else
		*x->tokens = next;
	x->cur = next;
	freesplit(x->words);
	return (2);
}

int	tok_handle_single(t_tok *x)
{
	int	res;

	res = tok_set_first(x, x->words[0]);
	freesplit(x->words);
	return (res);
}

int	tok_handle_multi(t_tok *x)
{
	t_token	*extra;
	int		res;

	extra = tok_build_extra(x);
	if (!extra)
		return (1);
	res = tok_set_first(x, x->words[0]);
	if (res)
	{
		free_tokenlist(extra);
		return (1);
	}
	tok_link_extra(x, extra);
	freesplit(x->words);
	return (0);
}

int	tok_step(t_tok *x)
{
	int	res;

	if (x->prev && x->prev->type == HEREDOC)
		return (0);
	res = tok_load_words(x);
	if (res)
		return (-1);
	if (x->wcount == 0)
		return (tok_handle_zero(x));
	if (x->wcount == 1)
		return (tok_handle_single(x));
	return (tok_handle_multi(x));
}
