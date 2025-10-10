/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 21:22:21 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/07 11:11:35 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

unsigned int	get_rgb(char *r, char *g, char *b);
void	parse_path(char *data);
char	**parse_rgb(char *r, char *g, char *b);

int	check_path(char *data)
{
	int fd;

	printf("Archivo de textura\n");
	fd = open(data, O_RDONLY);
	if (fd == -1)
	{
		printf("%s", data);
		return (-1);
	}
	close(fd);
	return (1);
}

t_tex *check_id(t_game *g, char *id)
{
    if (ft_strncmp(id, "NO", 2)	!= 0)
		return (&g->wall_north);
	else if (ft_strncmp(id, "SO", 2) != 0)
		return (&g->wall_south);
	else if (ft_strncmp(id, "WE", 2) != 0)
		return (&g->wall_west);
	else if (ft_strncmp(id, "EA", 2) != 0)
		return (&g->wall_east);
	else if (ft_strncmp(id, "F", 1) != 0)
		return (&g->floor);
	else if (ft_strncmp(id, "C", 1) != 0)
		return (&g->ceiling);
	else
		ft_error_exit("ID no válida");
	return (NULL);
}

void	parse_element(t_game *game, char *line)
{
	char	**splited;
	unsigned int color;
	t_tex	*tex;

	splited = ft_split(line, ' ');

	if (!splited[0] || !splited[1])
		error_handler(3);

	tex = check_id(game, splited[0]);
	if (tex)
		ft_error_exit("Elemento ya definido");
	if (!splited[2])
	{
		load_texture(game, tex, splited[1]);
	}
	else if (splited[2] && splited[3] && !splited[4])
	{
		color = get_rgb(splited[1], splited[2], splited[3]);
		if (color == COLOR_NONE)
			ft_error_exit("Color RGB no válido");
		tex->color = color;
	}
	else
		ft_error_exit("Error en la línea de elemento");
	
	ft_free_array((void ***)&splited);
}

unsigned int get_rgb(char *r, char *g, char *b)
{
	int i;
	char **rgb_str;
	unsigned int *rgb;

	rgb_str = parse_rgb(r, g, b);
	if (!rgb_str)
	{
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

	while (rgb[i] != NULL)
	{
		if (!ft_isnum(rgb[i]))
			return (NULL); // free rgb and return NULL	
		i++;
	}

	return (rgb);
}