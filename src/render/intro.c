/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intro.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:55:54 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/15 21:06:40 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	close_intro(int keycode, t_game *g)
{
	(void)keycode;
	if (g->render_state == INTRO && g->timer > MIN_TIME_TO_READ)
	{
		g->render_state = LOAD_LEVEL;
		ft_bzero(&g->keys, sizeof(t_keys));
		clean_screen(g);
		g->timer = 0;
		mlx_hook(g->win, 2, 1L << 0, key_press, g);
		mlx_hook(g->win, 3, 1L << 1, key_release, g);
	}
	return (0);
}

static int	read_intro(t_game *g)
{
	int			fd;
	char		*line;
	t_coords	coords;

	fd = open("./textures/intro.txt", O_RDONLY);
	if (fd == -1)
		return (0);
	coords.x = 160;
	coords.y = 60;
	line = get_next_line(fd);
	g->font.scale = 1.2;
	while (line)
	{
		render_text(g, line, coords);
		coords.y += g->font.char_h * 0.8 + 10;
		free(line);
		line = get_next_line(fd);
	}
	coords.y += g->font.char_h * 0.8 + 10;
	coords.x = WIDTH / 2 - ((int)ft_strlen("PRESS ANY BUTTON TO PLAY")
			* g->font.char_w * g->font.scale) / 2;
	if (g->timer > MIN_TIME_TO_READ && g->timer % 60 < 30)
		render_text(g, "PRESS ANY BUTTON TO PLAY", coords);
	close(fd);
	return (1);
}

void	show_intro(t_game *g)
{
	t_tex	intro;

	load_texture(g, &intro, "./textures/intro.xpm");
	draw_fullscreen_image(g, intro);
	read_intro(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	g->timer++;
	mlx_key_hook(g->win, close_intro, g);
	if (intro.img)
		mlx_destroy_image(g->mlx, intro.img);
}
