/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 09:07:23 by mnajem            #+#    #+#             */
/*   Updated: 2026/01/27 15:15:54 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int wout_qlen(char *s,int i,int j)
{
    int idx;
    int len;
    int quote;

    idx = i;
    len = 0;
    quote = 0;
    while(idx<j&&s[idx])
    {
        if(quote == 0 && (s[idx] == 34 || s[idx] == 39))
        {
            quote = s[idx];
            idx++;
            continue;
        }
        else if(quote == s[idx] && quote >0)
        {
            quote = 0;
            idx++;
            continue;
        }
        else
        {
            idx++;
            len++;
        }
    }
    if(quote != 0)
        return (-1);
    return len;
}
int endofword(char *s, int i)
{
    int quote;

    quote = 0;
    while (s[i])
    {
        if (quote == 0 && (zspace(s[i]) || s[i] == '|' || s[i] == '<' || s[i] == '>'))
            break;
        if (quote == 0 && (s[i] == 34 || s[i] == 39))
        {
            quote = s[i];
            i++;
            continue;
        }
        if (quote != 0 && s[i] == quote)
        {
            quote = 0;
            i++;
            continue;
        }
        i++;
    }
    if (quote != 0)
        return (-1);
    return (i);
}

char *nodevalue(t_toktype type,char *s,int *i)
{
    int j;
    int quote;
    int len;
    char *word;
    int idx;
    int endlen;
    
    if(type == WORD)
    {
        idx = 0;
        quote = 0;
        j = *i;
        endlen = endofword(s,*i);
        if(endlen < 0)
            return (NULL);
        len = wout_qlen(s,*i,endlen);
        if(len < 0)
            return (NULL);
        word = malloc(len+1);
        if(!word)
            return (NULL);
        while (j < endlen)
        {
            if(quote == 0&& (s[j] == 34 || s[j] == 39))
            {
                quote = s[j++];
                continue;
            }
            else if(quote == s[j] && quote >0)
            {
                quote = 0;
                j++;
                continue;
            }
            else
            {
                word[idx++] = s[j++];
            }
        }
        if(quote != 0)
        {
            free(word);
            return (NULL);
        }
        *i = j;
        word[idx] ='\0';
        return (word);
    }
    return (NULL);
}

t_toktype	ident(char *s,int *i)
{
	if (s[*i] == '|')
    {
        (*i)++;
		return (PIPE);
    }
	if (s[*i] == '<')
	{
		if (s[*i+1] && s[*i+1] == '<')
        {
            (*i)+=2;
			return (HEREDOC);
        }
        (*i)++;
		return (R_IN);
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] && s[*i + 1] == '>')
        {
            (*i)+=2;
			return (APPEND);
        }
        (*i)++;
		return (R_OUT);
	}
	return (WORD);
}

t_token	*tokenizer(char *line)
{
    int i;
    t_token *tokenlist;
    t_token *token;
    t_toktype type;
    char *value;
    
    tokenlist = NULL;
    i = 0;
    while(line[i])
    {
        token = NULL;
        while(line[i]&&zspace(line[i]))
            i++;
        if(line[i]&&!zspace(line[i]))
        {
            type = ident(line,&i);
            value = nodevalue(type,line,&i);
            if(type == WORD && !value)
            {
                free_tokenlist(tokenlist);
                printf("Unclosed quotation");
                return (NULL);
            }
            token = newtoken(type,value);
            addtoken(&tokenlist,token);
        }
    }
    return (tokenlist);
}
