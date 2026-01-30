/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 23:31:23 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/17 01:44:54 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void sig_ctrl(int sig)
{
    g_signal = sig;
    write(1,"\n",1);
    rl_on_new_line();
    rl_replace_line("",0);
    rl_redisplay();
}
void prepare_sig(void)
{
    struct sigaction sa;

    rl_catch_signals = 0;
    rl_catch_sigwinch = 0;
    sa.sa_handler = sig_ctrl;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT,&sa,NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
