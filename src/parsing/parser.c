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
char	**read_file(int fd);

#include "libft.h" // por ft_isspace y ft_strncmp

int is_elem_str(char *s)
{
    int i;

    if (!s)
        return (0);
    i = 0;
    while (ft_isspace(s[i]))
        i++;
    if (!ft_strncmp(&s[i], "NO ", 2))
        return (1);
    else if (!ft_strncmp(&s[i], "SO ", 2))
        return (1);
    else if (!ft_strncmp(&s[i], "WE ", 2))
        return (1);
    else if (!ft_strncmp(&s[i], "EA ", 2))
        return (1);
    else if (!ft_strncmp(&s[i], "C ", 1))
        return (1);
    else if (!ft_strncmp(&s[i], "F ", 1))
        return (1);
    return (0);
}

int is_map_str(char *s)
{
    int i = 0;

    if (!s)
        return (0);
    while (ft_isspace(s[i]))
        i++;
    if (s[i] == '\0') // línea vacía
        return (0);
    while (s[i])
    {
        if (!(s[i] == '0' || s[i] == '1' || s[i] == 'N' ||
              s[i] == 'S' || s[i] == 'E' || s[i] == 'W' ||
              ft_isspace(s[i])))
            return (0);
        i++;
    }
    return (1);
}

void    parse_arguments(int argc, char **argv)
{
    if (argc != 2)
        error_handler(0);
	else if (ft_strnstr(argv[1], ".cub", ft_strlen(argv[1])) == 0)
		error_handler(1);
}

// Devuelve todo el contenido del archivo
char	**read_file(int fd)
{
	char	**content;
	char	*line;

	content = NULL;
	line = get_next_line(fd);
	if (!line)
		error_handler(3);
	while (line)
	{
		ft_clean_line(&line);
		ft_append_array((void ***)&content, ft_strdup(line));
		free(line);
		line = get_next_line(fd);
	}
	return (content);
}

// Comprueba si el fichero del mapa es válido
void parse_file(t_game *game, char **content)
{
    int i = 0;
    int has_map = 0;

    if (!content || !content[0])
        error_handler(3);

    while (content[i])
    {
        if (is_elem_str(content[i]))
        {
			parse_element(game, content[i]);
            // Aquí podrías guardar la textura/color correspondiente
        }
        else if (is_map_str(content[i]))
        {
            has_map = 1;
        }
        else if (ft_strlen(content[i]) == 0) // línea vacía permitida
        {
            // ok
        }
        else
        {
            error_handler(3); // Línea inválida
        }
        i++;
    }
    if (!has_map)
        error_handler(3); // No hay mapa
}




/* TODO
*	- Parseo del path de los elementos
*	- Crear y rellenar structs de los elementos
*	- Parseo de mapa
*/