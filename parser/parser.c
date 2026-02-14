/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:43:28 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/15 00:56:01 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_argv(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_argv;
	int		i;

	count = 0;
	if (cmd->argv)
	{
		while (cmd->argv[count])
			count++;
	}
	new_argv = malloc(sizeof(char *) * (count + 2));
	if (!new_argv)
		return ;
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(arg);
	new_argv[count + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

void	add_redir(t_cmd *cmd, t_toktype type, char *target, char *hd_tmp)
{
	t_redir	*new;
	t_redir	*last;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->target = ft_strdup(target);
	new->heredoc_tmp = hd_tmp;
	new->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		last = cmd->redirs;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}

t_cmd	*parse_tokens(t_token *tokens)
{
	return (parse_tokens2(tokens));
}

t_cmd	*parse_line(char *line, t_env *env, int last_stat)
{
	return (parse_line2(line, env, last_stat));
}
