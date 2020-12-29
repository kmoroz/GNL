/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_bonus.h                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ksmorozo <ksmorozo@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/12/12 13:17:39 by ksmorozo      #+#    #+#                 */
/*   Updated: 2020/12/18 14:05:12 by ksmorozo      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stddef.h>
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 35
# endif

typedef struct	s_file_descriptor
{
	int							fd;
	char						*remainder;
	struct s_file_descriptor	*next;
}				t_fd;
typedef struct	s_read_data
{
	char					*buff;
	int						amount_read;
	int						read_successful;
}				t_read_data;
char			*ft_strjoin(char const *s1, char const *s2);
int				get_next_line(int fd, char **line);
char			*ft_strdup(const char *src);
size_t			ft_strlcpy(char *dest, const char *src, size_t size);
size_t			ft_strlen(const char *str);
char			*ft_strchr(const char *str, int ch);

#endif
