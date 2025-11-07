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

#define NO_TEXTURE "Error: could not load texture %s\n"
#define INVALID_SIZE "Error: texture %s has invalid size\n"

int	ft_isspace(int c)
{
	if (c == 9 || c == 10 || c == 11 || c == 12 || c == 13 || c == 32)
		return (1);
	return (0);
}

int	check_loaded_textures(t_game *game)
{
	if (!game->map_text[NO].img || !game->map_text[SO].img
		|| !game->map_text[WE].img || !game->map_text[EA].img
		|| !game->map_text[F].img || !game->map_text[C].img)
		return (0);
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

void	set_error_parsing(t_game *g, char *msg, char *path)
{
	if (g->render_state != HIGH_SCORE && g->render_state != WAITING_FOR_NAME
		&& g->render_state != SCORE_SAVED && g->render_state != INTRO)
	{
		g->render_state = GAME_OVER;
		if (g->level == 1)
			g->render_state = HIGH_SCORE;
	}
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
