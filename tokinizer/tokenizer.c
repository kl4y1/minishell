/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 09:07:23 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/12 06:17:20 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	endofword(char *s, int i)
{
	int	quote;

	quote = 0;
	while (s[i])
	{
		if (quote != 39 && s[i] == '\\')
		{
			if (!s[i + 1])
				return (-1);
			i += 2;
			continue ;
		}
		if (quote == 0 && (zspace(s[i]) || s[i] == '|' || s[i] == '<'
				|| s[i] == '>'))
			break ;
		if (quote == 0 && (s[i] == 34 || s[i] == 39))
		{
			quote = s[i];
			i++;
			continue ;
		}
		if (quote != 0 && s[i] == quote)
		{
			quote = 0;
			i++;
			continue ;
		}
		i++;
	}
	if (quote != 0)
		return (-1);
	return (i);
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

t_toktype	ident(char *s, int *i)
{
	if (s[*i] == '|')
	{
		(*i)++;
		return (PIPE);
	}
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

t_token	*tokenizer(char *line)
{
	int			i;
	t_token		*tokenlist;
	t_token		*token;
	t_toktype	type;
	char		*value;

	tokenlist = NULL;
	i = 0;
	while (line[i])
	{
		token = NULL;
		while (line[i] && zspace(line[i]))
			i++;
		if (line[i] && !zspace(line[i]))
		{
			type = ident(line, &i);
			value = nodevalue(type, line, &i);
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
			addtoken(&tokenlist, token);
		}
	}
	return (tokenlist);
}
