#include "get_next_line.h"

static char *extract_line(char *buffer)
{
	char *line;
	int i;

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
    char *new_line_pos;
    char *new_buffer;
    
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
		temp[bytes_read] = '\0';
		if (!buffer)
			buffer = ft_strdup(temp);
		else
		{
			buffer = ft_strjoin(buffer, temp);
			if (!buffer)
			{
				free(buffer);
				return (NULL);
			}
		}
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	if (bytes_read < 0 || (bytes_read == 0 && (!buffer || buffer[0] == '\0')))
	{
		free(buffer);
		return (NULL);
	}
	else if (bytes_read == 0 && !ft_strchr(buffer, '\n'))
	{
		line = ft_strdup(buffer);
		free(buffer);
		buffer = NULL;
		return (line);
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