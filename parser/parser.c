/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haabu-sa <haabu-sa@amman.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 19:43:28 by haabu-sa          #+#    #+#             */
/*   Updated: 2026/02/12 06:12:20 by haabu-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_cmd	*new_cmd(void)
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

static void	add_argv(t_cmd *cmd, char *arg)
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
		if (tokens->type == WORD)
			add_argv(cur, tokens->value);
		else if (tokens->type == PIPE)
		{
			cur->next = new_cmd();
			cur = cur->next;
		}
		else if (handle_redir(cur, &tokens))
			return (free_cmds(head), NULL);
		if (tokens)
			tokens = tokens->next;
	}
	return (head);
}

t_cmd	*parse_line(char *line, t_env *env, int last_stat)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenizer(line);
	if (!tokens)
		return (NULL);
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
	cmds = parse_tokens(tokens);
	free_tokenlist(tokens);
	return (cmds);
}
