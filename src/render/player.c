/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:03:09 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 14:03:12 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	set_direction(t_game *g, char c)
{
	if (c == 'N')
		return (g->spider.dir_x = 0, g->spider.dir_y = -1,
			g->spider.plane_x = 0.66, g->spider.plane_y = 0, 0);
	if (c == 'S')
		return (g->spider.dir_x = 0, g->spider.dir_y = 1, g->spider.plane_x =
			-0.66, g->spider.plane_y = 0, 0);
	if (c == 'E')
		return (g->spider.dir_x = 1, g->spider.dir_y = 0, g->spider.plane_x = 0,
			g->spider.plane_y = 0.66, 0);
	if (c == 'W')
		return (g->spider.dir_x = -1, g->spider.dir_y = 0,
			g->spider.plane_x = 0, g->spider.plane_y = -0.66, 0);
	if (c == 'P')
		return (g->spider.dir_x = -1, g->spider.dir_y = 0,
			g->spider.plane_x = 0, g->spider.plane_y = 0.66, 0);
	return (-1);
}

void	draw_hand(t_game *g)
{
	t_ray	ray;
	t_pos	p;
	t_tex	hand;

	if (!g->spider.hand || !g->spider.hand[0].addr || !g->spider.hand[1].addr)
		return ;

	hand = g->spider.hand[g->spider.state];

	int draw_width = hand.width * SCALE_SPRITE;
	int draw_height = hand.height * SCALE_SPRITE;

	ray.draw_start_x = GAME_WIDTH / 2 - draw_width / 2;
	ray.draw_start_y = HEIGHT - draw_height;

	p.y = -1;
	while (++p.y < draw_height)
	{
		p.x = -1;
		while (++p.x < draw_width)
		{
			// mapeo inverso: de pantalla → textura
			ray.tx = (int)(p.x / SCALE_SPRITE);
			ray.ty = (int)(p.y / SCALE_SPRITE);

			ray.color = *(unsigned int *)(hand.addr
				+ ray.ty * hand.line_len
				+ ray.tx * (hand.bpp / 8));

			if ((ray.color & 0x00FFFFFF) != 0) // ignorar fondo transparente
				put_pixel(g, ray.draw_start_x + p.x, ray.draw_start_y + p.y, ray.color);
		}
	}
}

