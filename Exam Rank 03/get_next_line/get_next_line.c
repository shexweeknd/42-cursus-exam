#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif

int	ft_strchr(char *str, char c)
{
	int	index;

	if (!str)
		return (0);
	index = 0;
	while (str[index])
	{
		if (str[index] == c)
			return (1);
		index++;
	}
	return (0);
}

size_t	ft_strlen(char *str)
{
	size_t	result;

	if (!str)
		return (0);
	result = 0;
	while (str[result])
		result++;
	return (result);
}

char	*ft_strjoin(char *remains, char *buffer)
{
	char	*result;
	int		i;
	int		j;

	if (!remains && !buffer)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(remains) + ft_strlen(buffer)
				+ 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	if (remains)
	{
		while (remains[i])
			result[j++] = remains[i++];
	}
	i = 0;
	while (buffer[i])
		result[j++] = buffer[i++];
	result[ft_strlen(remains) + ft_strlen(buffer)] = '\0';
	free(remains);
	return (result);
}

char	*push_line(char *buffer)
{
	char	*result;
	int		i;

	if (!buffer)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i++;
	result = malloc(sizeof(char) * (i + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		result[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
	{
		result[i] = '\n';
		result[i + 1] = '\0';
		return (result);
	}
	result[i] = '\0';
	return (result);
}

char	*get_remains(char *remains)
{
	int		i;
	int		j;
	char	*result;

	if (!remains)
		return (NULL);
	i = 0;
	while (remains[i] && remains[i] != '\n')
		i++;
	if (remains[i] == '\0')
	{
		free(remains);
		return (NULL);
	}
	result = malloc(sizeof(char) * (ft_strlen(&remains[i]) + 1));
	if (!result)
		return (NULL);
	i++;
	j = 0;
	while (remains[i])
		result[j++] = remains[i++];
	result[j] = '\0';
	free(remains);
	return (result);
}

char	*get_next_line(int fd)
{
	static char	*remains;
	char		*result;
	char		buffer[BUFFER_SIZE + 1];
	int			count;

	if (fd < 0 || !BUFFER_SIZE)
		return (NULL);
	count = 1;
	buffer[0] = '\0';
	while (!ft_strchr(buffer, '\n') && count)
	{
		count = read(fd, buffer, BUFFER_SIZE);
		buffer[count] = '\0';
		remains = ft_strjoin(remains, buffer);
	}
	result = push_line(remains);
	remains = get_remains(remains);
	if (result[0] == '\0')
	{
		free(result);
		return (NULL);
	}
	return (result);
}

/*
int	main(void)
{
	int fd;
	char *line;

	fd = open("./test.txt", O_RDONLY);
	if (fd < 0)
		return (0);
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}*/

/**
 * EXPLICATION DE L'ALGORITHME
 * 1
	- lire la ligne dans le fichier avec read dans buffer jusqu'a ce que la ligne lue contient '\n' et q'on arrive a lire dans le fichier
 * 2
	- join les lignes lues avec buffer pour avoir une ligne 'remains' contenant les possibles restes a chaque fois qu'on lit
 * 3
	- prendre les char devant le \\n puis le mettre dans une valeur de retour 'result'
 * 4 - prendre les eventuels restes puis les mettre dans 'remains'
 */