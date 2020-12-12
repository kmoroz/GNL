/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_bonus.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 13:18:50 by ksmorozo      #+#    #+#                 */
/*   Updated: 2020/12/12 13:33:36 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h> //open
#include <unistd.h> //read
#include <stdlib.h> //free
#include "get_next_line.h"

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
			ft_strlcpy(remainder, new_line_ptr + 1, ft_strlen(new_line_ptr));
		}
		else
		{
			*line = ft_strdup(remainder);
			ft_bzero(remainder, ft_strlen(remainder));
		}
	else
		*line = (char*)malloc(1);
	return (new_line_ptr);
}

int		get_next_line(int fd, char **line)
{
	char		buff[BUFF_SIZE + 1];
	int			amount_read;
	char		*new_line_ptr;
	static char *remainder;
	char		*temp;

	new_line_ptr = check_remainder(remainder, line);
	while (!new_line_ptr && (amount_read = read(fd, buff, BUFF_SIZE)))
	{
		buff[amount_read] = '\0';
		if ((new_line_ptr = ft_strchr(buff, '\n')))
		{
			*new_line_ptr = '\0';
			remainder = ft_strdup(new_line_ptr + 1);
		}
		temp = *line;
		*line = ft_strjoin(temp, buff);
		free(temp);
	}
	if (ft_strlen(*line) || amount_read)
		return (1);
	else
		return (0);
}

int		main()
{
	char	*line;
	int		fd;
	
	fd = open("abc.txt", O_RDONLY);
	while (get_next_line(fd, &line))
		printf("%s", line);
}
