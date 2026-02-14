/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 20:15:41 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/15 00:35:35 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_tokens(t_token *tokens)
{
	return (validate_tokens2(tokens));
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
