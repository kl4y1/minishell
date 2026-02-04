/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellmain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 20:31:34 by mnajem            #+#    #+#             */
/*   Updated: 2026/02/04 04:46:20 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int main(int argc, char **argv, char **envp)
{
    (void)argc; (void)argv;

    char    *shell;
    t_cmd   *cmds;
    int     status;
    t_env *env;
    int last_stat;

    last_stat = 0;
    env = envptoenv(envp);
    status = 0;
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
            cmds = parse_line(shell);
        }
        if (cmds)
        {
            int save_in = dup(STDIN_FILENO);
            int save_out = dup(STDOUT_FILENO);
            int ret;

            ret = -1;
            if (!cmds->next)
            {
                if (do_redirs(cmds->redirs) == 0)
                    ret = builtin(cmds->argv, &env, last_stat);
                else
                    ret = 1;
            }
            if (ret == -1)
                status = pipeline(cmds, &env, &last_stat);
            else
            {
                status = ret;
                last_stat = ret;
            }
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
