/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fresh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 01:31:02 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 23:32:39 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// locked
void	fresh_screen(void)
{
	char	*clear;

	if (tgetent(NULL, getenv("TERM")) != 1)
		return ;
	clear = tgetstr("cl", NULL);
	if (clear)
		tputs(clear, 1, ft_putchar);
}
