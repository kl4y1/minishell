/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*handle_word_token(char *line, int *i, t_token *tokenlist)
{
	t_toktype	type;
	char		*value;
	t_token		*token;

	type = ident(line, i);
	value = nodevalue(type, line, i);
	if (type == WORD && !value)
	{
		free_tokenlist(tokenlist);
		ft_putstr_fd("minishell: unclosed quotation\n", 2);
		return (NULL);
	}
	token = newtoken(type, value);
	if (!token)
	{
		free(value);
		free_tokenlist(tokenlist);
		return (NULL);
	}
	return (token);
}

t_token	*tokenizer2(char *line)
{
	int		i;
	t_token	*tokenlist;
	t_token	*token;

	tokenlist = NULL;
	i = 0;
	while (line[i])
	{
		token = NULL;
		while (line[i] && zspace(line[i]))
			i++;
		if (line[i] && !zspace(line[i]))
		{
			token = handle_word_token(line, &i, tokenlist);
			if (!token)
				return (NULL);
			addtoken(&tokenlist, token);
		}
	}
	return (tokenlist);
}
