/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:56:01 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*validate_and_expand(t_token *tokens, t_env *env, int last_stat)
{
	if (validate_tokens(tokens))
	{
		free_tokenlist(tokens);
		return (NULL);
	}
	if (expand_tokens(&tokens, env, last_stat))
	{
		free_tokenlist(tokens);
		return (NULL);
	}
	return (tokens);
}

t_cmd	*parse_line2(char *line, t_env *env, int last_stat)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenizer(line);
	if (!tokens)
		return (NULL);
	tokens = validate_and_expand(tokens, env, last_stat);
	if (!tokens)
		return (NULL);
	cmds = parse_tokens2(tokens);
	free_tokenlist(tokens);
	return (cmds);
}

static int	process_token(t_cmd **cur, t_cmd **head, t_token **tokens)
{
	(void)head;
	if ((*tokens)->type == WORD)
		add_argv(*cur, (*tokens)->value);
	else if ((*tokens)->type == PIPE)
	{
		(*cur)->next = new_cmd();
		*cur = (*cur)->next;
	}
	else if (handle_redir(*cur, tokens))
		return (1);
	return (0);
}

t_cmd	*parse_tokens2(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*cur;

	head = NULL;
	cur = NULL;
	while (tokens)
	{
		if (!cur)
		{
			cur = new_cmd();
			if (!cur)
				return (free_cmds(head), NULL);
			if (!head)
				head = cur;
		}
		if (process_token(&cur, &head, &tokens))
			return (free_cmds(head), NULL);
		if (tokens)
			tokens = tokens->next;
	}
	return (head);
}
