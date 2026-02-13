/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pid	*pid_node(pid_t pid)
{
	t_pid	*node;

	node = malloc(sizeof(t_pid));
	if (!node)
		exit_if_error("malloc");
	node->pid = pid;
	node->next = NULL;
	return (node);
}

void	pid_add_back(t_pid **pid_list, t_pid *node)
{
	t_pid	*cur;

	if (!pid_list || !node)
		return ;
	if (!*pid_list)
	{
		*pid_list = node;
		return ;
	}
	cur = *pid_list;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

void	free_pid_list(t_pid **pid_list)
{
	t_pid	*cur;
	t_pid	*next;

	cur = *pid_list;
	while (cur)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}
	*pid_list = NULL;
}

t_cmd	*getg_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}
