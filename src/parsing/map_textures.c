/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:56:15 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/10 19:29:10 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static t_tex	*get_tex_id(t_game *g, char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (&g->wall_north);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		return (&g->wall_south);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		return (&g->wall_west);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		return (&g->wall_east);
	else if (ft_strncmp(line, "F ", 2) == 0)
		return (&g->floor);
	else if (ft_strncmp(line, "C ", 2) == 0)
		return (&g->ceiling);
	if (!validate_line(line))
		ft_error_exit("Error: Invalid line in texture definitions\n");
	return (NULL);
}

static unsigned int	get_color(char *r, char *g, char *b)
{
	int	red;
	int	green;
	int	blue;

	red = ft_atoi(r);
	green = ft_atoi(g);
	blue = ft_atoi(b);
	if (red < 0 || red > 255 || green < 0 || green > 255 || blue < 0
		|| blue > 255)
		return (COLOR_NONE);
	return ((red << 16) | (green << 8) | blue);
}

static void	load_color(t_tex *tex, char **splited)
{
	unsigned int	color;
	char			*r;
	char			*g;
	char			*b;

	if (ft_memlen(splited) != 4)
		ft_error_exit("Error en la carga de color");
	r = ft_strtrim(splited[1], ",\n\r\t ");
	g = ft_strtrim(splited[2], ",\n\r\t ");
	b = ft_strtrim(splited[3], ",\n\r\t ");
	if (!r || !g || !b)
		ft_error_exit("Error en la carga de color");
	color = get_color(r, g, b);
	free(r);
	free(g);
	free(b);
	if (color == COLOR_NONE)
		ft_error_exit("Color RGB no válido");
	tex->color = color;
}

void	load_map_textures(t_game *g, char **content)
{
	int		i;
	t_tex	*tex;
	char	**splited;
	size_t	len_splited;

	i = -1;
	while (content[++i])
	{
		tex = get_tex_id(g, content[i]);
		if (!tex)
			continue ;
		splited = ft_split(content[i], ' ');
		if (!splited)
			ft_error_exit("Error en la línea de textura");
		len_splited = ft_memlen((const void **)splited);
		if (len_splited == 2)
			load_texture(g, tex, splited[1]);
		else if (len_splited == 4)
			load_color(tex, splited);
		ft_free_array((void ***)&splited);
	}
	if (!check_loaded_textures(g))
		ft_error_exit("Error: Not all textures/colors were defined");
}
