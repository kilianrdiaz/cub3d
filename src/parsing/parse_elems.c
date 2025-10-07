/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 21:22:21 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/17 21:34:11 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

unsigned int	get_rgb(char *r, char *g, char *b);
void	parse_path(char *data);
char	**parse_rgb(char *r, char *g, char *b);

void	save_element(t_game *game, char *id, char *path, unsigned int color)
{
	t_elem	*new_elem;

	new_elem = malloc(sizeof(t_elem));
	if (!new_elem)
		error_handler(3);
	new_elem->id = ft_strdup(id);
	if (!path)
	{
		new_elem->texture = NULL;
		new_elem->color = color;
	}
	else
	{
		//load_texture(game, new_elem->texture, path);
		new_elem->color = COLOR_NONE;
		// Textura guardada en estructura
	}

	ft_append_array((void ***)&game->elems, new_elem);

}

int	check_path(char *data)
{
	int fd;

	fd = open(data, O_RDONLY);
	if (fd == -1)
	{
		return (-1);
	}
	close(fd);
	return (1);
}

int check_id(t_elem **elems, char *id)
{
    int i;

    if (!id)
        return (-1);
    if (!elems)
    {
        printf("elems es NULL\n");
        return (1);
    }
    i = 0;
    while (elems[i])
    {
        if (ft_strncmp(elems[i]->id, id, ft_strlen(id)) == 0)
		{
			printf("ID repetida detectada: %s\n", id);
			return (-1);
		}
        i++;
    }
    return (0);
}

void	parse_element(t_game *game, char *line)
{
	int		i;
	char	**splited;
	unsigned int color;

	splited = ft_split(line, ' ');
	
    printf("estoy aqui\n");
	if (!splited[0] || !splited[1])
		error_handler(3);

	if (check_id(game->elems, splited[0]) == -1)
	{
		printf("ID repetida: %s\n", splited[0]);
		error_handler(3);
	}
	if (!splited[2])
	{
		if (check_path(splited[1]) == -1)
			error_handler(3);
		// Guardar en struct
		save_element(game, splited[0], splited[1], COLOR_NONE);
	}
	else if (splited[2] && splited[3] && !splited[4])
	{
		color = get_rgb(splited[1], splited[2], splited[3]);
		if (color == COLOR_NONE)
			error_handler(3);
		save_element(game, splited[0], NULL, color);
	}
	else
		error_handler(3);

	i = 0;
	while (splited[i])
		free(splited[i++]);
	free(splited);	
}

unsigned int get_rgb(char *r, char *g, char *b)
{
	int i;
	char **rgb_str;
	unsigned int *rgb;

	rgb_str = parse_rgb(r, g, b);
	if (!rgb_str)
	{
		printf("rgb_str es NULL\n");
		return (COLOR_NONE);
	}

	rgb = malloc(3 * sizeof(unsigned int));
	if (!rgb)
		return (COLOR_NONE);

	i = 0;
	while (i < 3 && rgb_str[i])
	{
		rgb[i] = (unsigned int)ft_atoi(rgb_str[i]);
		if (rgb[i++] > 255)
		{
			//free rgb_str
			//free rgb_value
			return (COLOR_NONE);
		}
	}
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

char	**parse_rgb(char *r, char *g, char *b)
{
	int		i;
	char	**rgb;
	
	rgb = NULL;
	i = 0;
	
	ft_append_array((void ***)&rgb, ft_strdup(ft_strtrim(r,  ",\n\r\t ")));
	ft_append_array((void ***)&rgb, ft_strdup(ft_strtrim(g,  ",\n\r\t ")));
	ft_append_array((void ***)&rgb, ft_strdup(ft_strtrim(b,  ",\n\r\t ")));
	ft_append_array((void ***)&rgb, NULL);

	while (rgb[i] != NULL)
	{
		if (!ft_isnum(rgb[i]))
			return (NULL); // free rgb and return NULL	
		i++;
	}

	return (rgb);
}