/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fresh.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 01:31:02 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/17 01:32:44 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void fresh_screen(void)
{
    char *clear;

    if (tgetent(NULL, getenv("TERM")) != 1)
        return;
    clear = tgetstr("cl", NULL);
    if (clear)
        tputs(clear, 1, putchar);
}
