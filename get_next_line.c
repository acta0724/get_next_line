/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiwasa <kiwasa@student.42.jp>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 04:11:35 by kiwasa            #+#    #+#             */
/*   Updated: 2024/11/03 04:13:07 by kiwasa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*append_to_buffer(char *buffer, char *temp, int bytes_read)
{
	char	*new_buffer;

	temp[bytes_read] = '\0';
	if (!buffer)
		buffer = ft_strdup(temp);
	else
	{
		new_buffer = ft_strjoin(buffer, temp);
		if (!new_buffer)
		{
			free(buffer);
			return (NULL);
		}
		buffer = new_buffer;
	}
	return (buffer);
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

static char	*update_buffer(char *buffer)
{
	char	*new_line_pos;
	char	*new_buffer;

	new_line_pos = ft_strchr(buffer, '\n');
	if (!new_line_pos)
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = ft_strdup(new_line_pos + 1);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	free(buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;
	char		temp[BUFFER_SIZE + 1];
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while ((bytes_read = read(fd, temp, BUFFER_SIZE)) > 0)
	{
		buffer = append_to_buffer(buffer, temp, bytes_read);
		if(!buffer)
			return (NULL);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	if (bytes_read < 0 || (bytes_read == 0 && (!buffer || buffer[0] == '\0')))
	{
		free(buffer);
		return (NULL);
	}
	line = extract_line(buffer);
	buffer = update_buffer(buffer);
	return (line);
}

// #include <stdio.h>
// #include <fcntl.h>
// int main()
// {
// 	char *line;
// 	int fd;

// 	fd = open("test.txt", O_RDONLY);
// 	while ((line = get_next_line(fd)))
// 	{
// 		printf("%s\n", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }