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

static void	draw_fullscreen_image(t_game *g, t_tex *tex)
{
	t_pos	p;
	t_pos	src_pos;
	char	*src;
	int		color;

	p.y = -1;
	while (++p.y < HEIGHT)
	{
		p.x = -1;
		while (++p.x < WIDTH)
		{
			src_pos.x = p.x * tex->width / WIDTH;
			src_pos.y = p.y * tex->height / HEIGHT;
			src = tex->addr + (src_pos.y * tex->line_len + src_pos.x * (tex->bpp
						/ 8));
			color = *(unsigned int *)src;
			put_pixel(g, p.x, p.y, color);
		}
	}
}

static int	close_intro(int keycode, t_game *g)
{
	(void)keycode;
	if (g->show_intro)
	{
		g->show_intro = 0;
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
	p.x = 250;
	p.y = 60;
	line = get_next_line(fd);
	g->font.scale = 0.6;
	while (line)
	{
		render_text(g, line, p);
		p.y += g->font.char_h * 0.4 + 10;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (1);
}

int	show_intro(t_game *g)
{
	t_tex	intro;

	g->show_intro = 1;
	load_texture(g, &intro, "./textures/intro.xpm");
	draw_fullscreen_image(g, &intro);
	read_intro(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	mlx_key_hook(g->win, close_intro, g);
	mlx_destroy_image(g->mlx, intro.img);
	return (0);
}
