/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_tok_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 23:57:35 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/08 23:57:41 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tok_free_list(t_token *list)
{
	free_tokenlist(list);
	return (NULL);
}

t_token	*tok_build_extra(t_tok *x)
{
	t_token	*list;
	t_token	*node;
	char	*dup;
	int		i;

	list = NULL;
	i = 1;
	while (x->words[i])
	{
		dup = ft_strdup(x->words[i]);
		if (!dup)
			return (tok_free_list(list));
		node = newtoken(WORD, dup);
		if (!node)
		{
			free(dup);
			return (tok_free_list(list));
		}
		addtoken(&list, node);
		i++;
	}
	return (list);
}

int	tok_set_empty(t_tok *x)
{
	char	*dup;

	dup = ft_strdup("");
	if (!dup)
		return (1);
	free(x->cur->value);
	x->cur->value = dup;
	return (0);
}

int	tok_set_first(t_tok *x, char *word)
{
	char	*dup;

	dup = ft_strdup(word);
	if (!dup)
		return (1);
	free(x->cur->value);
	x->cur->value = dup;
	return (0);
}

void	tok_link_extra(t_tok *x, t_token *extra)
{
	t_token	*next;
	t_token	*tail;

	next = x->cur->next;
	x->cur->next = extra;
	tail = tok_tail(extra);
	tail->next = next;
	x->cur = tail;
}
