/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 18:48:02 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/19 20:48:19 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	ft_isspace(int c)
{
	if (c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32)
		return (1);
	return (0);
}

int	check_loaded_textures(t_game *game)
{
	t_tex	tex;
	int		i;

	i = -1;
	while (++i < 6)
	{
		tex = game->map_text[i];
		if (!tex.img && tex.color == COLOR_NONE)
			return (0);
	}
	return (1);
}

int	validate_line(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (!(line[i] == '0' || line[i] == '1' || line[i] == 'N'
				|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W'
				|| line[i] == ' ' || line[i] == 'B' || line[i] == 'L'
				|| line[i] == 'P'))
			return (0);
	}
	return (1);
}

void	load_texture(t_game *g, t_tex *tex, char *path)
{
	if (!g || !tex || !path)
		return ;
	tex->img = mlx_xpm_file_to_image(g->mlx, path, &tex->width, &tex->height);
	if (!tex->img)
	{
		ft_printf_fd(STDERR_FILENO, "Error: could not load texture %s\n", path);
		exit(1);
	}
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	if (tex->width <= 0 || tex->height <= 0)
	{
		ft_printf_fd(STDERR_FILENO, "Error: texture %s has invalid size\n",
			path);
		exit(1);
	}
	tex->color = COLOR_NONE;
}
