/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_bonus.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 13:18:50 by ksmorozo      #+#    #+#                 */
/*   Updated: 2020/12/18 17:20:34 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h> //read
#include <stdlib.h> //free
#include "get_next_line_bonus.h"

char		*check_remainder(char *remainder, char **line)
{
	char *new_line_ptr;

	new_line_ptr = remainder ? ft_strchr(remainder, '\n') : NULL;
	if (remainder)
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
			while (*remainder)
				*remainder++ = '\0';
		}
	else
	{
		*line = malloc(1);
		**line = '\0';
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

int			get_line(int fd, char **line, char **remainder)
{
	char		*new_line_ptr;
	char		*temp;
	t_read_data read_data;

	new_line_ptr = check_remainder(*remainder, line);
	while (!new_line_ptr && (!read_data.buff || read_data.amount_read))
	{
		read_data = get_read_data(fd);
		if (fd < 0 || !line || BUFFER_SIZE < 1 || !read_data.read_successful)
			return (-1);
		new_line_ptr = ft_strchr(read_data.buff, '\n');
		if (new_line_ptr)
		{
			if (*remainder)
				free(*remainder);
			*new_line_ptr = '\0';
			*remainder = ft_strdup(new_line_ptr + 1);
		}
		temp = *line;
		*line = ft_strjoin(*line, read_data.buff);
		free(temp);
		free(read_data.buff);
	}
	return (!*remainder || !read_data.amount_read ? 0 : 1);
}

t_fd		*ft_lstnew(int fd)
{
	t_fd *new_element;

	new_element = (t_fd *)malloc(sizeof(t_fd));
	if (!new_element)
		return (NULL);
	new_element->fd = fd;
	new_element->remainder = NULL;
	new_element->next = NULL;
	return (new_element);
}

int			get_next_line(int fd, char **line)
{
	static t_fd		*head;
	t_fd			*current;

	if (fd < 0 || !line)
		return (-1);
	if (!head)
		head = ft_lstnew(fd);
	current = head;
	while (current->fd != fd)
	{
		if (!current->next)
			current->next = ft_lstnew(fd);
		current = current->next;
	}
	return (get_line(current->fd, line, &current->remainder));
}
/*
** #include <stdio.h>
** #include <fcntl.h> //open
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
