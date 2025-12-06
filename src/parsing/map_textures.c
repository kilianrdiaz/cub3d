/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_textures.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:56:15 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/26 21:46:04 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define ERR_LINE_TEXT "Error en la línea de textura %s\n"

static t_tex	*get_tex_id(t_game *g, char *line)
{
	t_tex	*tex;

	tex = NULL;
	if (ft_strncmp(line, "NO ", 3) == 0)
		tex = &g->map_text[NO];
	else if (ft_strncmp(line, "SO ", 3) == 0)
		tex = &g->map_text[SO];
	else if (ft_strncmp(line, "WE ", 3) == 0)
		tex = &g->map_text[WE];
	else if (ft_strncmp(line, "EA ", 3) == 0)
		tex = &g->map_text[EA];
	else if (ft_strncmp(line, "F ", 2) == 0)
		tex = &g->map_text[F];
	else if (ft_strncmp(line, "C ", 2) == 0)
		tex = &g->map_text[C];
	else if (!is_map_str(line) && line[0] != '\0')
		return (set_error_parsing(g, ERR_LINE_TEXT, line), NULL);
	if (g->level > 1 && tex && tex->img)
	{
		mlx_destroy_image(g->mlx, tex->img);
		tex->img = NULL;
	}
	return (tex);
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

static unsigned int	get_rgb_color(char **splited)
{
	unsigned int	color;
	char			**rgb;
	char			*str;
	int				x;

	rgb = NULL;
	x = 0;
	while (splited[++x])
	{
		str = ft_strtrim(splited[x], ",\n\r\t ");
		if (!str)
			return (ft_free_array((void ***)&color), COLOR_NONE);
		ft_append_array((void ***)&rgb, str);
	}
	if (x == 3)
		color = get_color(rgb[0], rgb[1], rgb[2]);
	else
		color = COLOR_NONE;
	ft_free_array((void ***)&rgb);
	return (color);
}

void	load_map_textures(t_game *g, char **content)
{
	int		i;
	t_tex	*tex;
	char	**splited;

	i = -1;
	while (content[++i])
	{
		tex = get_tex_id(g, content[i]);
		if (!tex)
			continue ;
		splited = ft_split(content[i], ' ');
		if (!splited)
			return (set_error_parsing(g, ERR_LINE_TEXT, content[i]));
		if (ft_memlen((const void **)splited) == 2)
			load_texture(g, tex, splited[1]);
		else
		{
			tex->color = get_rgb_color(splited);
			if (tex->color == COLOR_NONE)
				set_error_parsing(g, ERR_LINE_TEXT, content[i]);
		}
		ft_free_array((void ***)&splited);
	}
}
