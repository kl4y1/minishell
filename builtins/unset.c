/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 15:33:13 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/27 20:11:01 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int valid_ident(char *s)
{
    int i;

    i = 1;
    if(!s || !s[0])
        return 0;
    if(ft_isalpha(s[0]) || s[0] == '_')
    {
        while(s[i])
        {
            if(!ft_isalnum(s[i]) && s[i] != '_')
                return 0;
            i++;
        }
    }
    else
        return 0;
    return 1;
}

void	unset_key(t_env **env, char *key)
{
	t_env	*temp;
	t_env	*to_free;

	temp = NULL;
	if (!env || !*env || !key)
		return;
	while (*env)
	{
		if ((*env)->key && ft_strcmp((*env)->key, key) == 0)
		{
			to_free = *env;
			if (temp)
				temp->next = (*env)->next;
			else
				*env = (*env)->next;
			free(to_free->key);
			free(to_free->value);
			free(to_free);
			return;
		}
		temp = *env;
		env = &((*env)->next);
	}
}


int our_unset(char **argv, t_env **env)
{
    int i;
    int alo;

    alo = 0;
    i = 0;
    if (ft_strcmp(argv[0], "unset") == 0)
    {
        i = 1;
        while(argv[i])
        {
            if(!valid_ident(argv[i]))
            {
                write(2,"unset : invalid identifier\n",27);
                alo = 1;
            }
            else
                unset_key(env,argv[i]);
            i++;
        }
    }
    return alo;
}
