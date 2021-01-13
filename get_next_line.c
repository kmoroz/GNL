/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/09 09:44:38 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/01/13 19:24:28 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

void		ft_bzero(void *s, size_t n)
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

char		*check_remainder(char *remainder, char **new_line_ptr)
{
	char *str;

	*new_line_ptr = ft_strchr(remainder, '\n');
	if (*new_line_ptr)
	{
		**new_line_ptr = '\0';
		str = ft_strdup(remainder);
		(*new_line_ptr)++;
		ft_strlcpy(remainder, *new_line_ptr, ft_strlen(*new_line_ptr) + 1);
	}
	else
	{
		str = ft_strdup(remainder);
		ft_bzero(remainder, ft_strlen(remainder));
	}
	return (str);
}

t_read_data	get_read_data(int fd)
{
	t_read_data read_data;

	read_data.buff = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!read_data.buff)
	{
		read_data.read_successful = 0;
		return (read_data);
	}
	read_data.amount_read = read(fd, read_data.buff, BUFFER_SIZE);
	if (read_data.amount_read < 0)
	{
		read_data.read_successful = 0;
		return (read_data);
	}
	read_data.buff[read_data.amount_read] = '\0';
	read_data.read_successful = 1;
	return (read_data);
}

void		get_new_line(char **new_line_ptr, t_read_data read_data,
char *remainder)
{
	*new_line_ptr = ft_strchr(read_data.buff, '\n');
	if (*new_line_ptr)
	{
		**new_line_ptr = '\0';
		(*new_line_ptr)++;
		ft_strlcpy(remainder, *new_line_ptr, ft_strlen(*new_line_ptr) + 1);
	}
}

int			get_next_line(int fd, char **line)
{
	t_read_data		read_data;
	char			*new_line_ptr;
	static char		remainder[BUFFER_SIZE];
	char			*temp;

	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	read_data.amount_read = 1;
	*line = check_remainder(remainder, &new_line_ptr);
	if (!*line)
		return (-1);
	while (!new_line_ptr && read_data.amount_read)
	{
		read_data = get_read_data(fd);
		if (!read_data.read_successful)
			return (-1);
		get_new_line(&new_line_ptr, read_data, remainder);
		temp = *line;
		*line = ft_strjoin(*line, read_data.buff);
		if (!*line)
			return (-1);
		free(temp);
		free(read_data.buff);
	}
	return (read_data.amount_read ? 1 : 0);
}
