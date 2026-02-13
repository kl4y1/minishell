/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:15:41 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 06:14:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_operator(t_toktype type)
{
	return (type == PIPE || type == R_IN || type == R_OUT
		|| type == APPEND || type == HEREDOC);
}

int	validate_tokens(t_token *tokens)
{
	t_token	*prev;

	if (!tokens)
		return (1);
	prev = NULL;
	while (tokens)
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

static void	free_redir(t_redir *redir)
{
	t_redir	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->target);
		if (redir->heredoc_tmp)
		{
			unlink(redir->heredoc_tmp);
			free(redir->heredoc_tmp);
		}
		free(redir);
		redir = tmp;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->argv)
		{
			i = 0;
			while (cmds->argv[i])
				free(cmds->argv[i++]);
			free(cmds->argv);
		}
		free_redir(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}
