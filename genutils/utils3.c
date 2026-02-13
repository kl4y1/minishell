/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 18:43:44 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/13 18:30:41 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_toktype type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token_back(t_token **lst, t_token *new_tok)
{
	t_token	*tmp;

	if (!lst || !new_tok)
		return ;
	if (*lst == NULL)
		*lst = new_tok;
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_tok;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

int	handle_redir(t_cmd *cmd, t_token **tok)
{
	t_toktype	type;
	char		*hd_tmp;

	type = (*tok)->type;
	*tok = (*tok)->next;
	if (!*tok || (*tok)->type != WORD)
		return (1);
	hd_tmp = NULL;
	if (type == HEREDOC)
	{
		hd_tmp = make_heredoc((*tok)->value);
		if (!hd_tmp)
			return (1);
	}
	add_redir(cmd, type, (*tok)->value, hd_tmp);
	return (0);
}
