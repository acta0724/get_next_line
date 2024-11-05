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

static char	*free_fd_list(t_fd_list **fd_list, int fd)
{
	t_fd_list	*current;
	t_fd_list	*previous;

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

static char *update_buffer(char *buffer)
{
    size_t  i;
    char    *new_buffer;

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
	char				*line;
	char				temp[BUFFER_SIZE + 1];
	int					bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	current = get_fd_list(&head, fd);
	if (!current)
		return (NULL);
	while ((bytes_read = read(fd, temp, BUFFER_SIZE)) > 0)
	{
		current->buffer = append_to_buffer(current->buffer, temp, bytes_read);
		if (!current->buffer)
			return (free_fd_list(&head, fd));
		if (ft_strchr(current->buffer, '\n'))
			break ;
	}
	if (bytes_read < 0 || (bytes_read == 0 && (!current->buffer || current->buffer[0] == '\0')))
		return (free_fd_list(&head, fd));
	line = extract_line(current->buffer);
	current->buffer = update_buffer(current->buffer);
	return (line);
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