/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/01 12:00:00 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/01 16:13:51 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_redirs(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->target);
		free(redir->heredoc_tmp);
		free(redir);
		redir = next;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*next;
	int		i;

	while (cmds)
	{
		next = cmds->next;
		if (cmds->argv)
		{
			i = 0;
			while (cmds->argv[i])
				free(cmds->argv[i++]);
			free(cmds->argv);
		}
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = next;
	}
}

static t_redir	*redir_new(t_toktype type, char *target, char *heredoc_tmp)
{
	t_redir	*node;

	node = malloc(sizeof(t_redir));
	if (!node)
		return (NULL);
	node->type = type;
	node->target = target;
	node->heredoc_tmp = heredoc_tmp;
	node->next = NULL;
	return (node);
}

static void	redir_add_back(t_redir **lst, t_redir *node)
{
	t_redir	*cur;

	if (!lst || !node)
		return ;
	if (!*lst)
	{
		*lst = node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static t_cmd	*cmd_new(char **argv, t_redir *redirs)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	if (!node)
		return (NULL);
	node->argv = argv;
	node->redirs = redirs;
	node->next = NULL;
	return (node);
}

static void	cleanup_partial(char **argv, t_redir *redirs)
{
	int	i;

	if (argv)
	{
		i = 0;
		while (argv[i])
			free(argv[i++]);
		free(argv);
	}
	free_redirs(redirs);
}

static void	cmd_add_back(t_cmd **lst, t_cmd *node)
{
	t_cmd	*cur;

	if (!lst || !node)
		return ;
	if (!*lst)
	{
		*lst = node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static char	*make_heredoc( char *limiter)
{
	char	template[] = "/tmp/minish_heredocXXXXXX";
	int		fd;
	char	*line;
	char	*path;

	fd = mkstemp(template);
	if (fd < 0)
		return (NULL);
	path = ft_strdup(template);
	if (!path)
	{
		close(fd);
		return (NULL);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (path);
}

static int	syntax_error( char *msg)
{
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	return (0);
}

static int	count_words_and_validate(t_token *start, int *word_count, int *redir_count)
{
	t_token	*cur;

	*word_count = 0;
	*redir_count = 0;
	cur = start;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == WORD)
			(*word_count)++;
		else
		{
			if (!cur->next || cur->next->type != WORD)
				return (syntax_error("syntax error near redirection"));
			(*redir_count)++;
			cur = cur->next;
		}
		cur = cur->next;
	}
	return (1);
}

static t_cmd	*build_cmd(t_token *start)
{
	int		word_count;
	int		redir_count;
	char	**argv;
	t_redir	*redirs;
	t_token	*cur;
	int		i;

	if (!count_words_and_validate(start, &word_count, &redir_count))
		return (NULL);
	if (word_count == 0 && redir_count == 0)
	{
		syntax_error("syntax error near pipe");
		return (NULL);
	}
	argv = ft_calloc(word_count + 1, sizeof(char *));
	if (!argv)
		return (NULL);
	redirs = NULL;
	cur = start;
	i = 0;
	while (cur && cur->type != PIPE)
	{
		if (cur->type == WORD)
		{
			argv[i] = ft_strdup(cur->value);
			if (!argv[i])
				return (cleanup_partial(argv, redirs), NULL);
			i++;
		}
		else
		{
			char	*target = ft_strdup(cur->next->value);
			char	*tmp = NULL;
			if (!target)
				return (cleanup_partial(argv, redirs), NULL);
			if (cur->type == HEREDOC)
			{
				tmp = make_heredoc(cur->next->value);
				if (!tmp)
					return (cleanup_partial(argv, redirs), NULL);
			}
			{
				t_redir	*node = redir_new(cur->type, target, tmp);
				if (!node)
				{
					free(target);
					free(tmp);
					return (cleanup_partial(argv, redirs), NULL);
				}
				redir_add_back(&redirs, node);
			}
			cur = cur->next;
		}
		cur = cur->next;
	}
	return (cmd_new(argv, redirs));
}

t_cmd	*parse_line(char *line)
{
	t_token	*tokens;
	t_token	*cur;
	t_token	*segment_start;
	t_cmd	*cmds;
	t_cmd	*node;

	tokens = tokenizer(line);
	if (!tokens)
		return (NULL);
	cmds = NULL;
	cur = tokens;
	segment_start = cur;
	if (cur && cur->type == PIPE)
	{
		free_tokenlist(tokens);
		syntax_error("syntax error near pipe");
		return (NULL);
	}
	while (cur)
	{
		if (cur->type == PIPE)
		{
			node = build_cmd(segment_start);
			if (!node)
			{
				free_tokenlist(tokens);
				free_cmds(cmds);
				return (NULL);
			}
			cmd_add_back(&cmds, node);
			segment_start = cur->next;
		}
		cur = cur->next;
	}
	node = build_cmd(segment_start);
	if (!node)
	{
		free_tokenlist(tokens);
		free_cmds(cmds);
		return (NULL);
	}
	cmd_add_back(&cmds, node);
	free_tokenlist(tokens);
	return (cmds);
}
