/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 00:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/15 00:37:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_operator(t_toktype type)
{
	return (type == PIPE || type == R_IN || type == R_OUT
		|| type == APPEND || type == HEREDOC);
}

static int	check_operator_error(t_token *tokens, t_token *prev)
{
	if (!prev && tokens->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			2);
		return (1);
	}
	if (prev && is_operator(prev->type) && is_operator(tokens->type))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
		return (1);
	}
	return (0);
}

static int	check_redir_target(t_token *tokens)
{
	if (tokens->type == R_IN || tokens->type == R_OUT
		|| tokens->type == APPEND || tokens->type == HEREDOC)
	{
		if (!tokens->next || tokens->next->type != WORD)
		{
			ft_putstr_fd(
				"minishell: syntax error near unexpected token `newline'\n",
				2);
			return (1);
		}
	}
	return (0);
}

int	validate_tokens2(t_token *tokens)
{
	t_token	*prev;

	if (!tokens)
		return (1);
	prev = NULL;
	while (tokens)
	{
		if (check_operator_error(tokens, prev))
			return (1);
		if (check_redir_target(tokens))
			return (1);
		prev = tokens;
		tokens = tokens->next;
	}
	if (prev && prev->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (1);
	}
	return (0);
}
