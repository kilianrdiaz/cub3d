/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:22 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 13:56:26 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_fullscreen_image(t_game *g, t_tex *tex)
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

static int	game(t_game *g)
{
	update_player_position(g);
	clean_screen(g);
	render_floor_and_ceiling(g);
	render_wall(g);
	render_sprites(g);
	update_bombs(g);
	move_lizards(g);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	draw_hand(g, GAME_WIDTH / 2);
	if (g->spider.state == ATTACKING)
		spider_attack(g);
	g->timer += 0.1;
	return (0);
}

int	render(t_game *g)
{
	if (g->render_state == INTRO)
		return (show_intro(g));
	if (g->render_state == PLAYING)
		return (game(g));
	if (g->render_state == HIGH_SCORE || g->render_state == WAITING_FOR_NAME || g->render_state == SCORE_SAVED)
		return (show_high_scores(g));
	return (0);
}
