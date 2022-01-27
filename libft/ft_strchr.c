/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: fbes <fbes@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/10/26 19:47:41 by fbes          #+#    #+#                 */
/*   Updated: 2022/01/26 00:00:44 by fbes          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

/**
 * Search for a character in a string
 * @param *s	The string to search in
 * @param c		The character to look for
 * @return		A pointer to the character in the string, NULL if not found
 */
char	*ft_strchr(const char *s, int c)
{
	char	ch;

	ch = (char)c;
	while (*s != '\0' || ch == '\0')
	{
		if (*s == ch)
			return ((char *)s);
		s++;
	}
	return (NULL);
}
