/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_bonus.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 13:18:50 by ksmorozo      #+#    #+#                 */
/*   Updated: 2020/12/15 12:40:03 by ksmorozo      ########   odam.nl         */
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

int		get_line(int fd, char **line, char **remainder)
{
	char		buff[BUFFER_SIZE + 1];
	int			amount_read;
	char		*new_line_ptr;
	char		*temp;
	char		*temp_rem;

	new_line_ptr = check_remainder(*remainder, line);
	while (!new_line_ptr && (amount_read = read(fd, buff, BUFFER_SIZE)))
	{
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
	if (ft_strlen(*line) || amount_read)
		return (1);
	else
		return (0);
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

	if (fd < 0 || !*line)
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

int	main()
{
	char	*line;
	int		fd1;
	int		fd2;
	int		count;
	
	count = 1;
	fd1 = open("shrek small.txt", O_RDONLY);
	fd2 = open("abc.txt", O_RDONLY);
	while (get_next_line(fd1, &line) && count <= 3)
	{
		printf("%s\n", line);
		free(line);
		count++;
	}
		free(line);
	while (get_next_line(fd2, &line) && count <= 6)
	{
		printf("%s\n", line);
		free(line);
		count++;
	}
		free(line);
	while (get_next_line(fd1, &line) && count <= 9)
	{
		printf("%s\n", line);
		free(line);
		count++;
	}
	free(line);
}
