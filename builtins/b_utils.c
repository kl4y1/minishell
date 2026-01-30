/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_utils.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:43:15 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/30 16:47:56 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//needs rebuilding all of it is wrong
#include "minishell.h"

int	isbuiltin(char *s)
{
	if (!s)
		return (0);
	if (ft_strcmp(s, "echo") == 0)
		our_echo();
	if (ft_strcmp(s, "cd") == 0)
		our_cd();
	if (ft_strcmp(s, "pwd") == 0)
		our_pwd();
	if (ft_strcmp(s, "export") == 0)
		out_export();
	if (ft_strcmp(s, "unset") == 0)
		out_unset();
	if (ft_strcmp(s, "env") == 0)
		our_env();
	if (ft_strcmp(s, "exit") == 0)
		our_exit();
	return (0);
}
