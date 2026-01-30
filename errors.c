/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:47:35 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/17 19:41:59 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_path_error(char **cmd, char **paths)
{
	perror("command incorrect or not found");
	freesplit(cmd);
	freesplit(paths);
	exit(127);
}

void	error_execve(char **cmd, char *execpath)
{
	perror("execve error");
	freesplit(cmd);
	free(execpath);
	exit(126);
}

void	exit_if_error(char *error)
{
	perror(error);
	exit(1);
}

void	exit_usage(char *message)
{
	write(2, message, ft_strlen(message));
	write(2, "\n", 1);
	exit(1);
}
