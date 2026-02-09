/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeworkutils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 23:22:21 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/08 20:18:36 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*getg_last_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

pid_t	get_last_cmd(t_cmd *cmd, t_env **env, t_pid **pid_list, int last_stat)
{
	pid_t	pid;

	pid = exec_final_command(cmd, env, last_stat);
	pid_add_back(pid_list, pid_node(pid));
	return (pid);
}

void	get_middle_cmds(t_cmd *cmds, t_env **env, t_pid **pid_list,
		int last_stat)
{
	t_cmd	*cur;
	pid_t	pid;

	cur = cmds;
	while (cur && cur->next)
	{
		pid = paf(cur, env, last_stat);
		pid_add_back(pid_list, pid_node(pid));
		cur = cur->next;
	}
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

int	wait_pids(t_pid *pid_list, pid_t last_pid, int *last_stat)
{
	int		status;
	int		exit_code;
	t_pid	*cur;
	void	(*old_int)(int);
	void	(*old_quit)(int);

	exit_code = 0;
	old_int = signal(SIGINT, SIG_IGN);
	old_quit = signal(SIGQUIT, SIG_IGN);
	cur = pid_list;
	while (cur)
	{
		if (waitpid(cur->pid, &status, 0) == -1)
		{
			perror("waitpid");
			cur = cur->next;
			continue ;
		}
		if (cur->pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				exit_code = 128 + WTERMSIG(status);
				if (WTERMSIG(status) == SIGQUIT)
					write(2, "Quit: 3\n", 8);
				else if (WTERMSIG(status) == SIGINT)
					write(2, "\n", 1);
			}
			*last_stat = exit_code;
		}
		cur = cur->next;
	}
	signal(SIGINT, old_int);
	signal(SIGQUIT, old_quit);
	return (exit_code);
}

int	do_redirs(t_redir *redir)
{
	int	file;

	while (redir)
	{
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
		if (file < 0)
		{
			ft_putstr_fd("minishell: ", 2);
			if (redir->type == HEREDOC)
				perror(redir->heredoc_tmp);
			else
				perror(redir->target);
			return (1);
		}
		if (redir->type == R_IN || redir->type == HEREDOC)
		{
			if (dup2(file, STDIN_FILENO) < 0)
			{
				perror("dup2");
				close(file);
				return (1);
			}
		}
		else
		{
			if (dup2(file, STDOUT_FILENO) < 0)
			{
				perror("dup2");
				close(file);
				return (1);
			}
		}
		close(file);
		redir = redir->next;
	}
	return (0);
}
