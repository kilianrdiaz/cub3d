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

int is_elem_str(char *line)
{
    /*if (!line)
        return (0);*/
    if (!ft_strncmp(line, "NO ", 2))
        return (1);
    else if (!ft_strncmp(line, "SO ", 2))
        return (1);
    else if (!ft_strncmp(line, "WE ", 2))
	return (1);
    else if (!ft_strncmp(line, "EA ", 2))
        return (1);
    else if (!ft_strncmp(line, "C ", 1))
        return (1);
    else if (!ft_strncmp(line, "F ", 1))
        return (1);
    return (0);
}

void    parse_arguments(int argc, char **argv)
{
    int i;

    if (argc != 2)
        error_handler(0);
	i = -1;
    while (argv[1][++i] && argv[1][i] != '.')
        continue ;
    if (argv[1][i] || ft_strncmp(&argv[1][i], ".cub", 4) != 0)
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
		ft_append_array((void ***)&content, line);
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
	{
    	error_handler(3);
	}
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
			get_map(game, content, i);
			check_map_validity(game);
			break; // el mapa es lo último, ya no seguimos parseando
		}
        else if (is_empty_line(content[i])) // línea vacía permitida
        {
            // ok
        }
        else
        {
			printf("linea invalida: %s, Len: %ld\n", content[i], ft_strlen(content[i]));
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