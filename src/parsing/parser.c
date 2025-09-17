/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:04:55 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/17 21:28:26 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

// Comprueba si los argumentos son válidos
char	*join_line(char *old_ret, char *line);
char	*read_file(int fd, char *ret);


void    parse_arguments(int argc, char **argv)
{
    int fd;
	int len;
	char	*ptr;

    if (argc != 2)
        error_handler(0);
    len = ft_strlen(argv[1]);
	if (len < 4)
		error_handler(2);
	ptr = ft_strnstr(&argv[1][len - 4], ".cub", len);
	if (!ptr)
		error_handler(1);
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
        error_handler(2);
	// TODO - Parseo del mapa
	//parse_file(fd, game);
}
// Devuelve todo el contenido del archivo
char	*read_file(int fd, char *ret)
{
	char	*line;

	if (!ret)
		ret = ft_strdup("");
	while ((line = get_next_line(fd)) != NULL)
	{
		ret = join_line(ret, line);
		free(line);
	}
	return (ret);
}

char	*join_line(char *old_ret, char *line)
{
	char	*ret;

	ret = ft_strjoin(old_ret, line);
	free(old_ret);
	return (ret);
}
// Comprueba si el fichero del mapa es válido
void    parse_file(int fd)
{
    char    *content;
	int		i;
	int		filled;

    content = NULL;
    content = read_file(fd, content);
	i = 0;
	filled = 0; //TODO

	while (content[i] && filled)
	{
		while (ft_isspace(content[i]))
			i++;
		if (!ft_strncmp(&content[i], "NO", 2))
			parse_element(&content[i + 2], "NO");
		else if (!ft_strncmp(&content[i], "SO", 2))
			parse_element(&content[i + 2], "SO");
		else if (!ft_strncmp(&content[i], "WE", 2))
			parse_element(&content[i + 2], "WE");
		else if (!ft_strncmp(&content[i], "EA", 2))
			parse_element(&content[i + 2], "EA");
		else if (!ft_strncmp(&content[i], "C", 1))
			parse_element(&content[i + 1], "C");
		else if (!ft_strncmp(&content[i], "F", 1))
			parse_element(&content[i + 1], "F");
		else
			error_handler(3);
		while (content[i] && content[i] != '\n')
			i++;
	}
	
    // TODO - Parseo de mapa
}



/* TODO
*	- Parseo del path de los elementos
*	- Crear y rellenar structs de los elementos
*	- Parseo de mapa
*/