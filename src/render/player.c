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

static t_ray	ray_hand(t_tex hand)
{
	t_ray	ray;

	ray.draw_end_x = hand.width * SCALE_SPRITE;
	ray.draw_end_y = hand.height * SCALE_SPRITE;
	ray.draw_start_x = GAME_WIDTH / 2 - ray.draw_end_x / 2;
	ray.draw_start_y = HEIGHT - ray.draw_end_y;
	return (ray);
}

void	draw_hand(t_game *g)
{
	t_ray	ray;
	t_pos	p;
	t_tex	hand;

	if (!g->spider.hand || !g->spider.hand[0].addr || !g->spider.hand[1].addr)
		return ;
	hand = g->spider.hand[g->spider.state];
	ray = ray_hand(hand);
	p.y = -1;
	while (++p.y < ray.draw_end_y)
	{
		p.x = -1;
		while (++p.x < ray.draw_end_x)
		{
			// mapeo inverso: de pantalla → textura
			ray.tx = (int)(p.x / SCALE_SPRITE);
			ray.ty = (int)(p.y / SCALE_SPRITE);
			ray.color = *(unsigned int *)(hand.addr + ray.ty * hand.line_len
					+ ray.tx * (hand.bpp / 8));
			if ((ray.color & 0x00FFFFFF) != 0) // ignorar fondo transparente
				put_pixel(g, ray.draw_start_x + p.x, ray.draw_start_y + p.y,
					ray.color);
		}
	}
}
