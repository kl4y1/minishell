/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 09:07:23 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:35:35 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	endofword(char *s, int i)
{
	return (endofword2(s, i, 0));
}

char	*nodevalue(t_toktype type, char *s, int *i)
{
	int		j;
	int		len;
	char	*word;

	if (type == WORD)
	{
		j = endofword(s, *i);
		if (j < 0)
			return (NULL);
		len = j - *i;
		word = malloc(len + 1);
		if (!word)
			return (NULL);
		ft_memcpy(word, s + *i, len);
		word[len] = '\0';
		*i = j;
		return (word);
	}
	return (NULL);
}

static t_toktype	check_redirect(char *s, int *i)
{
	if (s[*i] == '<')
	{
		if (s[*i + 1] && s[*i + 1] == '<')
		{
			(*i) += 2;
			return (HEREDOC);
		}
		(*i)++;
		return (R_IN);
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] && s[*i + 1] == '>')
		{
			(*i) += 2;
			return (APPEND);
		}
		(*i)++;
		return (R_OUT);
	}
	return (WORD);
}

t_toktype	ident(char *s, int *i)
{
	if (s[*i] == '|')
	{
		(*i)++;
		return (PIPE);
	}
	return (check_redirect(s, i));
}

t_token	*tokenizer(char *line)
{
	return (tokenizer2(line));
}
