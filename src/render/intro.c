/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intro.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 22:55:54 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/03 22:55:58 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	close_intro(int keycode, t_game *g)
{
	(void)keycode;
	if (g->render_state == INTRO)
	{
		g->render_state = PLAYING;
		ft_bzero(&g->keys, sizeof(t_keys));
		clean_screen(g);
		mlx_hook(g->win, 2, 1L << 0, key_press, g);
		mlx_hook(g->win, 3, 1L << 1, key_release, g);
	}
	return (0);
}

static int	read_intro(t_game *g)
{
	int		fd;
	char	*line;
	t_pos	p;

	fd = open("./textures/intro.txt", O_RDONLY);
	if (fd < 0)
		return (0);
	p.x = 160;
	p.y = 60;
	line = get_next_line(fd);
	g->font.scale = 1.2;
	while (line)
	{
		render_text(g, line, p);
		p.y += g->font.char_h * 0.8 + 10;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (1);
}

int	show_intro(t_game *g)
{
	t_tex	intro;

	g->render_state = INTRO;
	load_texture(g, &intro, "./textures/intro.xpm");
	draw_fullscreen_image(g, &intro);
	read_intro(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	mlx_key_hook(g->win, close_intro, g);
	mlx_destroy_image(g->mlx, intro.img);
	return (0);
}
