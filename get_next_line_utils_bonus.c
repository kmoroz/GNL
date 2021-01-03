/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils_bonus.c                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 13:20:37 by ksmorozo      #+#    #+#                 */
/*   Updated: 2020/12/12 13:21:56 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char	*ft_strchr(const char *str, int ch)
{
	unsigned char	*modifiable_str;

	modifiable_str = (unsigned char *)str;
	if (!modifiable_str)
	return (NULL);
	while (*modifiable_str != ch)
	{
		if (*modifiable_str == '\0')
			return (NULL);
		modifiable_str++;
	}
	return ((char*)modifiable_str);
}

size_t	ft_strlen(const char *str)
{
	size_t count;

	count = 0;
	while (str[count] != '\0')
		count++;
	return (count);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t count;

	count = 0;
	if (size == 0)
		return (ft_strlen(src));
	while (src[count] != '\0' && count < (size - 1))
	{
		dest[count] = src[count];
		count++;
	}
	dest[count] = '\0';
	return (ft_strlen(src));
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*new_str;
	size_t	count;

	count = 0;
	new_str = (char*)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (new_str == NULL)
		return (NULL);
	while (count < ft_strlen(s1))
	{
		new_str[count] = s1[count];
		count++;
	}
	count = 0;
	while (count < ft_strlen(s2) + 1)
	{
		new_str[count + ft_strlen(s1)] = s2[count];
		count++;
	}
	return (new_str);
}

char	*ft_strdup(const char *src)
{
	char *dest;

	dest = (char*)malloc(ft_strlen(src) + 1);
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, src, ft_strlen(src) + 1);
	return (dest);
}
