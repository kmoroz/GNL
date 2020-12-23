/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/09 09:44:38 by ksmorozo      #+#    #+#                 */
/*   Updated: 2020/12/18 16:49:19 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h> //open
#include <unistd.h> //read
#include <stdlib.h> //free
#include <stdio.h>
#include "get_next_line.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	count;
	char	*str;

	str = s;
	count = 0;
	while (count != n)
	{
		str[count] = '\0';
		count++;
	}
}

void	*ft_calloc(size_t nitems, size_t size)
{
	void *pointer;

	pointer = malloc(nitems * size);
	if (pointer == NULL)
		return (NULL);
	ft_bzero(pointer, nitems * size);
	return (pointer);
}

char	*ft_strchr(const char *str, int ch)
{
	unsigned char	*modifiable_str;

	modifiable_str = (unsigned char *)str;
	while (*modifiable_str != ch)
	{
		if (*modifiable_str == '\0')
			return (NULL);
		modifiable_str++;
	}
	return ((char*)modifiable_str);
}

char	*check_remainder(char *remainder, char **line)
{
	char *new_line_ptr;

	new_line_ptr = NULL;
	if (remainder)
		if ((new_line_ptr = ft_strchr(remainder, '\n')))
		{
			*new_line_ptr = '\0';
			*line = ft_strdup(remainder);
			new_line_ptr++;
			ft_strlcpy(remainder, new_line_ptr, ft_strlen(new_line_ptr) + 1);
		}
		else
		{
			*line = ft_strdup(remainder);
			ft_bzero(remainder, ft_strlen(remainder));
		}
	else
		*line = ft_calloc(1, 1);
	return (new_line_ptr);
}

int		get_next_line(int fd, char **line)
{
	char		*buff;
	int			amount_read;
	char		*new_line_ptr;
	static char *remainder;
	char		*temp;

	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	amount_read = 1;
	new_line_ptr = check_remainder(remainder, line);
	while (!new_line_ptr && amount_read)
	{
		buff = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
		if (!buff)
			return (-1);
		amount_read = read(fd, buff, BUFFER_SIZE);
		if (amount_read < 0)
			return (-1);
		buff[amount_read] = '\0';
		if ((new_line_ptr = ft_strchr(buff, '\n')))
		{
			if (remainder)
				free(remainder);
			*new_line_ptr = '\0';
			remainder = ft_strdup(new_line_ptr + 1);
		}
		temp = *line;
		*line = ft_strjoin(*line, buff);
		free(temp);
	}
	return (!remainder || !amount_read ? 0 : 1);
}

/*
** int     main(int argc, char **argv)
** {
**     char    *line;
**     int     fd;
** 
**     if (argc == 2)
**         fd = open(argv[1], O_RDONLY);
** 	else
** 		fd = 0;
**     while (get_next_line(fd, &line))
**     {
**         printf("%s\n", line);
**         free(line);
**     }
**     free(line);
** }
*/