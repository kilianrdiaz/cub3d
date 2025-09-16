/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:04:55 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/12 19:20:44 by kroyo-di         ###   ########.fr       */
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
	//parse_map(fd);
}
// Devuelve todo el contenido del archivo
char	*read_file(int fd, char *ret)
{
	char	*line;
	int		chars_read;

	if (!ret)
		ret = (char *)malloc(sizeof(char));
	line = (char *)malloc((BUFFER_SIZE + 1) * sizeof(char));
	chars_read = -1;
	while (chars_read > 0)
	{
		chars_read = read(fd, line, BUFFER_SIZE);
		if (chars_read == -1)
		{
			free(line);
			return (NULL);
		}
		line[chars_read] = '\0';
		ret = join_line(ret, line);
	}
	free(line);
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
void    parse_map(int fd)
{
    char    *content;

    content = NULL;
    content = read_file(fd, content);

    // TODO - Parseo de elementos

    // TODO - Parseo de mapa
}

/*
* TODO
*
* Incorporar get_next_line completo
* Leer linea a linea el archivo de mapa
* Comprobar que todas las lineas estan bien escritas
* Comprobar que el mapa es correcto
*/
