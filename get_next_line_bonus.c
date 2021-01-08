/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_bonus.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 13:18:50 by ksmorozo      #+#    #+#                 */
/*   Updated: 2021/01/08 14:05:06 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "get_next_line_bonus.h"

char		*check_remainder(char *remainder, char **new_line_ptr)
{
	char *str;

	*new_line_ptr = ft_strchr(remainder, '\n');
	if (remainder)
		if (*new_line_ptr)
		{
			**new_line_ptr = '\0';
			str = ft_strdup(remainder);
			(*new_line_ptr)++;
			ft_strlcpy(remainder, *new_line_ptr, ft_strlen(*new_line_ptr) + 1);
		}
		else
			str = ft_strdup(remainder);
	else
	{
		str = malloc(1);
		if (!str)
			return (NULL);
		*str = '\0';
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

int			get_line(int fd, char **line, char **remainder)
{
	char		*new_line_ptr;
	char		*temp;
	t_read_data read_data;

	read_data.amount_read = 1;
	*line = check_remainder(*remainder, &new_line_ptr);
	while (!new_line_ptr && read_data.amount_read)
	{
		read_data = get_read_data(fd);
		if (!read_data.read_successful || !line)
			return (-1);
		new_line_ptr = ft_strchr(read_data.buff, '\n');
		if (new_line_ptr)
		{
			temp = *remainder;
			*new_line_ptr = '\0';
			*remainder = ft_strdup(new_line_ptr + 1);
			free(temp);
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

	if (fd < 0 || !line || BUFFER_SIZE < 1)
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
