/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:48:02 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 17:42:42 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define NO_TEXTURE "Error: could not load texture %s\n"
#define INVALID_SIZE "Error: texture %s has invalid size\n"

int	**get_visited_map(char **map)
{
	int	row;
	int	*visited_row;
	int	**visited;

	visited = NULL;
	row = -1;
	while (map[++row])
	{
		visited_row = ft_safe_calloc((ft_strlen(map[row])), sizeof(int));
		ft_append_array((void ***)&visited, visited_row);
	}
	return (visited);
}

static int	*get_textures_defined(char **content)
{
	int	i;
	int	*textures;

	i = -1;
	textures = ft_safe_calloc(sizeof(int), sizeof(int) * 6);
	while (content[++i])
	{
		if (ft_strncmp(content[i], "NO ", 3) == 0)
			textures[NO] = 1;
		else if (ft_strncmp(content[i], "SO ", 3) == 0)
			textures[SO] = 1;
		else if (ft_strncmp(content[i], "WE ", 3) == 0)
			textures[WE] = 1;
		else if (ft_strncmp(content[i], "EA ", 3) == 0)
			textures[EA] = 1;
		else if (ft_strncmp(content[i], "F ", 2) == 0)
			textures[F] = 1;
		else if (ft_strncmp(content[i], "C ", 2) == 0)
			textures[C] = 1;
		else if (!is_map_str(content[i]) && content[i][0] != '\0')
			return (free(textures), NULL);
	}
	return (textures);
}

int	all_textures_defined(char **content)
{
	int	*textures;

	textures = get_textures_defined(content);
	if (!textures)
	{
		free(textures);
		return (-1);
	}
	if (textures[NO] && textures[SO] && textures[WE] && textures[EA]
		&& textures[F] && textures[C])
	{
		free(textures);
		return (1);
	}
	free(textures);
	return (0);
}

void	set_error_parsing(t_game *g, char *msg, char *path)
{
	if (g->render_state != HIGH_SCORE && g->render_state != WAITING_FOR_NAME
		&& g->render_state != SCORE_SAVED && g->render_state != INTRO)
		g->render_state = HIGH_SCORE;
	if (msg)
		ft_printf_fd(2, msg, path);
}

void	load_texture(t_game *g, t_tex *tex, char *path)
{
	if (!g || !tex || !path)
		return ;
	tex->color = COLOR_NONE;
	tex->img = mlx_xpm_file_to_image(g->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
		return (set_error_parsing(g, NO_TEXTURE, path));
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	if (tex->width <= 0 || tex->height <= 0)
		return (set_error_parsing(g, INVALID_SIZE, path));
}
