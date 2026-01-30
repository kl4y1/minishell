/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:46:04 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/30 16:48:06 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_tokenlist(t_token *list)
{
    t_token *next;

    while (list)
    {
        next = list->next;
        free(list->value);
        free(list);
        list = next;
    }
}

char	*join_path(char *path, char *cmd)
{
	char	*res;
	int		len1;
	int		len2;
	int		lensum;

	len1 = ft_strlen(path);
	len2 = ft_strlen(cmd);
	lensum = len1 + len2;
	res = malloc(lensum + 2);
	if (!res)
		return (NULL);
	ft_memcpy(res, path, len1);
	res[len1] = '/';
	ft_memcpy(res + len1 + 1, cmd, len2);
	res[lensum + 1] = '\0';
	return (res);
}

void	freesplit(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	closefds(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}

void	waitpids(pid_t child1, pid_t child2)
{
	waitpid(child1, NULL, 0);
	waitpid(child2, NULL, 0);
}

int zspace(char c)
{
	if(c < 32)
		return 1;
	return 0;
}

int zchar(char c)
{
	if((c >= 65 && c<=90)||(c>=97 && c<=122))\
		return 1;
	return 0;
}

void	addtoken(t_token **lst, t_token *new)
{
	t_token	*temp;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	temp = *lst;
	while (temp->next)
	{
		temp = temp->next;
	}
	temp->next = new;
}

t_token *newtoken(t_toktype type, char *value)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (!new)
        return (NULL);
    new->type = type;
    new->value = value;
    new->next = NULL;
    return (new);
}

int checkflag(char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    if (!(s[0] == '-' && s[1] == 'n'))
        return (0);
    i = 2;
    while (s[i] == 'n')
        i++;
    if (s[i] == '\0')
        return (1);
    return (0);
}
