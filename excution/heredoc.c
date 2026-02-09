/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 03:20:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/09 20:14:20 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_path(int n)
{
	char	*idx;
	char	*path;

	idx = ft_itoa(n);
	if (!idx)
		return (NULL);
	path = ft_strjoin("/tmp/minish_heredoc_", idx);
	free(idx);
	return (path);
}

static int	open_heredoc_file(char **path)
{
	static int	n;
	int			fd;

	while (n < HEREDOC_MAX_TRIES)
	{
		*path = heredoc_path(n++);
		if (!*path)
			return (-1);
		fd = open(*path, O_CREAT | O_EXCL | O_WRONLY, 0600);
		if (fd >= 0)
			return (fd);
		free(*path);
		*path = NULL;
	}
	return (-1);
}

static char	*clean_limiter(char *limiter)
{
	char	*clean;
	int		i;
	int		j;

	clean = ft_calloc(ft_strlen(limiter) + 1, sizeof(char));
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (limiter[i])
	{
		if (limiter[i] != '\'' && limiter[i] != '"')
			clean[j++] = limiter[i];
		i++;
	}
	clean[j] = '\0';
	return (clean);
}

static int	fill_heredoc(int fd, char *clean)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			free(line);
			return (130);
		}
		if (!line)
		{
			break ;
		}
		if (ft_strcmp(line, clean) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

char	*make_heredoc(char *limiter)
{
	t_hd	hd;

	hd.path = NULL;
	hd.clean = clean_limiter(limiter);
	if (!hd.clean)
		return (NULL);
	hd.fd = open_heredoc_file(&hd.path);
	if (hd.fd < 0)
	{
		free(hd.clean);
		return (NULL);
	}
	hd_set_signals(&hd.old_int, &hd.old_quit);
	hd.res = fill_heredoc(hd.fd, hd.clean);
	hd_restore_signals(&hd.old_int, &hd.old_quit);
	hd_cleanup(&hd);
	return (hd.path);
}
