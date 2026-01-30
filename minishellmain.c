/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellmain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 20:31:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/30 16:45:29 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc; (void)argv;

    char    *shell;
    t_cmd   *cmds;
    int     status;

    fresh_screen();
    prepare_sig();

    while (1)
    {
        shell = readline(C_PURPLE "alo?$ " C_RESET);
        if (!shell)
        {
            printf("exit\n");
            break;
        }

        cmds = NULL;
        if (shell[0])
        {
            add_history(shell);
        }
        if (cmds)
        {
            int save_in = dup(STDIN_FILENO);
            int save_out = dup(STDOUT_FILENO);

            status = pipeline(cmds, envp); // envp temporarily
            dup2(save_in, STDIN_FILENO);
            close(save_in);
            dup2(save_out, STDOUT_FILENO);
            close(save_out);
        }
        free_cmds(cmds);
        free(shell);
    }
    return (status);
}
