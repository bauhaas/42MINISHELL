/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clorin <clorin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/21 09:54:36 by clorin            #+#    #+#             */
/*   Updated: 2020/11/24 10:59:37 by clorin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
**	returns 1 if there is '\n' in the string
**	otherwise return 0
*/

static int			nl_line(char *str)
{
	int				i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

/*
**	return a malloc string from the first line of str or str if none '\ n'
*/

static char			*recup_line(char *str)
{
	int				i;
	char			*dest;

	i = 0;
	if (!str)
	{
		dest = (char *)malloc(1);
		dest[0] = '\0';
		return (dest);
	}
	while (str[i] && str[i] != '\n')
		i++;
	dest = (char *)malloc(sizeof(char) * (i + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		dest[i] = str[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*
**	Remove the first line encountered and return a malloc
**  from the rest of the chain
**	free the old str
*/

static char			*save_static(char *str)
{
	int				i;
	int				j;
	char			*dest;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	j = 0;
	dest = (char *)malloc(sizeof(char) * (ft_strlen(str) - i));
	if (!dest)
		return (NULL);
	i += 1;
	while (str[i])
		dest[j++] = str[i++];
	dest[j] = '\0';
	free(str);
	return (dest);
}

/*
**	check the inputs of get_next_line()
**	and free buffer if not valid inputs
*/

static int			check_input(char *buffer, const int fd, char **line)
{
	if (fd < 0 || fd > MAX_FD || !line || BUFFER_SIZE <= 0 || !buffer ||
	read(fd, NULL, 0) < 0)
	{
		if (buffer)
			free(buffer);
		return (0);
	}
	return (1);
}

/*
**	1# We check fd, line and buffer_size: return -1 if problems.
**	2# We allocate a buffer of the size of buffer_size.
**	3# As long as str_static does not contain a '\ n'
***		and the read has not returned 0(result)
**		- we read a packet of BUFFER_SIZE oct.
**		- we add this package to the static variable.
**	4# we get the line to send back.
**	5# we trunk the static variable of the returned row.
**	6# If the reading returned 0 and static variable empty,
**		we returned 0, otherwise 1 because there is still something to read.
*/

int					get_next_line(const int fd, char **line)
{
	static char		*str_static[MAX_FD];
	char			*buffer;
	int				result;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!check_input(buffer, fd, line))
		return (-1);
	result = 1;
	while (!nl_line(str_static[fd]) && result != 0)
	{
		result = read(fd, buffer, BUFFER_SIZE);
		if (result == -1)
		{
			free(buffer);
			return (-1);
		}
		buffer[result] = '\0';
		str_static[fd] = strjoin_gnl(str_static[fd], buffer);
	}
	free(buffer);
	*line = recup_line(str_static[fd]);
	str_static[fd] = save_static(str_static[fd]);
	result = (result == 0 && ft_strlen(str_static[fd]) == 0) ? 0 : 1;
	return (result);
}
