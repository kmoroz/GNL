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

#include <unistd.h> //read
#include <stdlib.h> //free
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

char		*check_remainder(char *remainder, char **line)
{
	char *new_line_ptr;

	new_line_ptr = NULL;
	if (remainder)
	{
		new_line_ptr = ft_strchr(remainder, '\n');
		if (new_line_ptr)
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
	}
	else
	{
		*line = malloc(1);
		ft_bzero(*line, 1);
	}
	return (new_line_ptr);
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

int			get_line(char **new_line_ptr, t_read_data read_data, char **remainder)
{
	*new_line_ptr = ft_strchr(read_data.buff, '\n');
	if (*new_line_ptr)
	{
		if (*remainder)
			free(*remainder);
		**new_line_ptr = '\0';
		*remainder = ft_strdup(*new_line_ptr + 1);
		if (!*remainder)
			return (0);
	}
	return (1);
}

int			get_next_line(int fd, char **line)
{
	t_read_data		read_data;
	char			*new_line_ptr;
	static char		*remainder;
	char			*temp;
	int				line_assigned;

	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	new_line_ptr = check_remainder(remainder, line);
	read_data.amount_read = 1;
	while (!new_line_ptr && read_data.amount_read)
	{
		read_data = get_read_data(fd);
		if (!read_data.read_successful)
			return (-1);
		line_assigned = get_line(&new_line_ptr, read_data, &remainder);
		if (!line_assigned)
			return (-1);
		temp = *line;
		*line = ft_strjoin(*line, read_data.buff);
		free(temp);
	}
	return (!remainder || !read_data.amount_read ? 0 : 1);
}

#include <stdio.h>
#include <fcntl.h> //open
/*
** int     main(int argc, char **argv)
** {
**     char    *line;
**     int     fd;
**     fd = open("shrek small.txt", O_RDONLY);
**     while (get_next_line(fd, &line))
**     {
**         printf("%s\n", line);
**         free(line);
**     }
** 	printf("%s\n", line);
**     free(line);
** }
*/
