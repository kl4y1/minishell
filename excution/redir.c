/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 12:00:00 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 12:00:00 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redir_file(t_redir *redir)
{
	int	file;

	file = -1;
	if (redir->type == R_IN)
		file = open(redir->target, O_RDONLY);
	else if (redir->type == R_OUT)
		file = open(redir->target, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir->type == APPEND)
		file = open(redir->target, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (redir->type == HEREDOC)
	{
		file = open(redir->heredoc_tmp, O_RDONLY);
		if (file >= 0)
			unlink(redir->heredoc_tmp);
	}
	return (file);
}

static int	redir_error(t_redir *redir)
{
	ft_putstr_fd("minishell: ", 2);
	if (redir->type == HEREDOC)
		perror(redir->heredoc_tmp);
	else
		perror(redir->target);
	return (1);
}

static int	apply_dup2(int file, t_redir *redir)
{
	int	target_fd;

	if (redir->type == R_IN || redir->type == HEREDOC)
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(file, target_fd) < 0)
	{
		perror("dup2");
		close(file);
		return (1);
	}
	close(file);
	return (0);
}

int	do_redirs(t_redir *redir)
{
	int	file;

	while (redir)
	{
		file = open_redir_file(redir);
		if (file < 0)
			return (redir_error(redir));
		if (apply_dup2(file, redir))
			return (1);
		redir = redir->next;
	}
	return (0);
}
