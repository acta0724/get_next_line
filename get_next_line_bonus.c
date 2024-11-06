/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 03:03:39 by kiwasa            #+#    #+#             */
/*   Updated: 2024/11/07 06:12:10 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static t_fd_list	*get_fd_list(t_fd_list **fd_list, int fd)
{
	t_fd_list	*new_fd;

	new_fd = *fd_list;
	while (new_fd)
	{
		if (new_fd->fd == fd)
			return (new_fd);
		new_fd = new_fd->next;
	}
	new_fd = malloc(sizeof(t_fd_list));
	if (!new_fd)
		return (NULL);
	new_fd->fd = fd;
	new_fd->buffer = NULL;
	new_fd->next = *fd_list;
	*fd_list = new_fd;
	return (new_fd);
}

static char	*free_fd_list(t_fd_list **fd_list, int fd, char *temp)
{
	t_fd_list	*current;
	t_fd_list	*previous;

	if (temp)
		free(temp);
	current = *fd_list;
	previous = NULL;
	while (current)
	{
		if (current->fd == fd)
		{
			if (previous)
				previous->next = current->next;
			else
				*fd_list = current->next;
			free(current->buffer);
			free(current);
			return (NULL);
		}
		previous = current;
		current = current->next;
	}
	return (NULL);
}

static char	*extract_line(char *buffer)
{
	char	*line;
	int		i;

	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
		i++;
	if (buffer[i] == '\n')
		i++;
	line = malloc(sizeof(char) * (i + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] != '\n' && buffer[i] != '\0')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		line[i] = '\n';
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*update_buffer(char *buffer, char *temp)
{
	size_t	i;
	char	*new_buffer;

	if (temp)
		free(temp);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_strdup(buffer + i + 1);
	free(buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	static t_fd_list	*head;
	t_fd_list			*current;
	t_gnl_vars			vars;

	if (fd < 0 || BUFFER_SIZE <= 0 || !get_fd_list(&head, fd))
		return (NULL);
	current = get_fd_list(&head, fd);
	vars.temp = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!vars.temp)
		return (NULL);
	if (read_and_store(fd, current, &vars) == -1)
		return (free_fd_list(&head, fd, vars.temp));
	if (vars.bytes < 0)
		return (free_fd_list(&head, fd, vars.temp));
	if (vars.bytes == 0 && (!current->buffer || current->buffer[0] == '\0'))
		return (free_fd_list(&head, fd, vars.temp));
	vars.line = extract_line(current->buffer);
	current->buffer = update_buffer(current->buffer, vars.temp);
	return (vars.line);
}

// #include <stdio.h>
// #include <fcntl.h>
// int main(int argc, char **argv)
// {
// 	int fd;
// 	char *line;

// 	if (argc == 2)
// 	{
// 		fd = open(argv[1], O_RDONLY);
// 		while ((line = get_next_line(fd)))
// 		{
// 			printf("%s\n", line);
// 			free(line);
// 		}
// 		close(fd);
// 	}
// 	return (0);
// }