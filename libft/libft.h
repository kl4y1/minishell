/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mnajem <mnajem@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 18:50:30 by mnajem            #+#    #+#             */
/*   Updated: 2025/12/23 17:18:25 by mnajem           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
int		ft_strlen(char *s);
int		ft_strlcpy(char *dst, char *src, int size);
char	*ft_strdup(char *s);
char	*ft_substr(char *s, int start, int len);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strchr(char *s, int c);

int		ft_isprint(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
void	*ft_memset(void *s, int c, int n);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strrchr(char *s, int c);
int		ft_atoi(char *nptr);
void	ft_bzero(void *s, int n);
void	*ft_memchr(void *s, int c, int n);
int		ft_memcmp(void *s1, void *s2, int n);
void	*ft_memcpy(void *dest, void *src, int n);
void	*ft_memmove(void *dest, void *src, int n);
int		ft_isascii(int c);
int		ft_isalpha(int c);
int		ft_putchar(int c);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
char	*ft_strmapi(char *s, char (*f)(unsigned int, char));
char	**ft_split(char *s, char c);
void	*ft_calloc(int nmemb, int size);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
char	*ft_strtrim(char *s1, char *set);
char	*ft_strnstr(char *big, char *little, int len);
int		ft_strncmp(char *s1, char *s2, int n);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_itoa(int n);

#endif
