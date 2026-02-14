/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_backslash(char *s, int i, int quote)
{
	if (quote != 39 && s[i] == '\\')
	{
		if (!s[i + 1])
			return (-1);
		return (i + 2);
	}
	return (i);
}

static int	handle_quote(char *s, int i, int *quote)
{
	if (*quote == 0 && (s[i] == 34 || s[i] == 39))
	{
		*quote = s[i];
		return (i + 1);
	}
	if (*quote != 0 && s[i] == *quote)
	{
		*quote = 0;
		return (i + 1);
	}
	return (i);
}

static int	process_char(char *s, int i, int *quote)
{
	int	new_i;

	new_i = handle_backslash(s, i, *quote);
	if (new_i == -1)
		return (-1);
	if (new_i != i)
		return (new_i);
	if (*quote == 0 && (zspace(s[i]) || s[i] == '|' || s[i] == '<'
			|| s[i] == '>'))
		return (-2);
	new_i = handle_quote(s, i, quote);
	if (new_i != i)
		return (new_i);
	return (i + 1);
}

int	endofword2(char *s, int i, int quote)
{
	int	new_i;

	while (s[i])
	{
		new_i = process_char(s, i, &quote);
		if (new_i == -1)
			return (-1);
		if (new_i == -2)
			break ;
		i = new_i;
	}
	if (quote != 0)
		return (-1);
	return (i);
}
