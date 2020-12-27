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

#include <fcntl.h> //open
#include <unistd.h> //read
#include <stdlib.h> //free
#include "get_next_line_bonus.h"

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
	{
		*line = malloc(1);
		ft_bzero(*line, 1);
	}
	return (new_line_ptr);
}

int		get_line(int fd, char **line, char **remainder)
{
	char		*buff;
	int			amount_read;
	char		*new_line_ptr;
	char		*temp;
	char		*temp_rem;

	if (fd < 0 || !line || BUFFER_SIZE < 1)
		return (-1);
	amount_read = 1;
	new_line_ptr = check_remainder(*remainder, line);
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
			*new_line_ptr = '\0';
			temp_rem = *remainder;
			*remainder = ft_strdup(new_line_ptr + 1);
			free(temp_rem);
		}
		temp = *line;
		*line = ft_strjoin(*line, buff);
		if (!*line || amount_read == -1)
			return (-1);
		free(temp);
	}
	return (!*remainder || !amount_read ? 0 : 1);
}

t_fd	*ft_lstnew(int fd)
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

int		get_next_line(int fd, char **line)
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