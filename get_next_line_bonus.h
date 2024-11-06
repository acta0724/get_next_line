/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 03:06:42 by kiwasa            #+#    #+#             */
/*   Updated: 2024/11/07 06:12:05 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_fd_list
{
	int					fd;
	char				*buffer;
	struct s_fd_list	*next;
}	t_fd_list;

typedef struct s_gnl_vars
{
	char	*line;
	char	*temp;
	int		bytes;
}	t_gnl_vars;

char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char *s1, char *s2);
int		read_and_store(int fd, t_fd_list *current, t_gnl_vars *vars);
char	*add_buf(char *buffer, char *temp, int bytes);

#endif