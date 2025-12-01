/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:15:22 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/29 15:21:55 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_fullscreen_image(t_game *g, t_tex tex)
{
	t_pos	p;
	t_pos	src_pos;
	char	*src;
	int		color;

	if (!tex.img)
		return ;
	p.y = -1;
	while (++p.y < HEIGHT)
	{
		p.x = -1;
		while (++p.x < WIDTH)
		{
			src_pos.x = p.x * tex.width / WIDTH;
			src_pos.y = p.y * tex.height / HEIGHT;
			src = tex.addr + (src_pos.y * tex.line_len + src_pos.x * (tex.bpp
						/ 8));
			color = *(unsigned int *)src;
			put_pixel(g, p.x, p.y, color);
		}
	}
}

static void	game_pause(t_game *g)
{
	render_text(g, "PAUSE!", (t_coords){GAME_W / 2 - 100, HEIGHT / 2 - 20});
	if (g->keys.p)
	{
		g->render_state = PLAYING;
		g->keys.p = 0;
	}
}

void	game(t_game *g)
{
	if (g->render_state == PLAYING)
	{
		update_player_position(g, 0);
		if (g->spider.state == ATTACKING)
			spider_attack(g);
		update_bombs(g);
		move_lizards(g);
	}
	clean_screen(g);
	render_map(g);
	render_sprites(g);
	draw_hand(g, GAME_W / 2);
	render_stats(g);
	if (g->render_state == PAUSE)
		game_pause(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	if (g->render_state != PLAYING)
		return ;
	update_timer(g);
}
