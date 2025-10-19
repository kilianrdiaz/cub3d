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

static t_ray	ray_hand(t_tex hand, double spider_x)
{
	t_ray	ray;

	ft_bzero(&ray, sizeof(t_ray));
	ray.draw_end_x = hand.width * SCALE_SPRITE;
	ray.draw_end_y = hand.height * SCALE_SPRITE;
	ray.draw_start_x = spider_x - ray.draw_end_x / 2;
	ray.draw_start_y = HEIGHT - ray.draw_end_y;
	return (ray);
}

void	draw_hand(t_game *g, int x)
{
	t_pos	p;
	t_tex	hand;
	t_ray	l_ray;

	if (!g->spider.hand || !g->spider.hand[0].addr || !g->spider.hand[1].addr)
		return ;
	hand = g->spider.hand[g->spider.state];
	l_ray = ray_hand(hand, x);
	p.y = -1;
	while (++p.y < l_ray.draw_end_y)
	{
		p.x = -1;
		while (++p.x < l_ray.draw_end_x)
		{
			l_ray.src.x = (int)(p.x / SCALE_SPRITE);
			l_ray.src.y = (int)(p.y / SCALE_SPRITE);
			l_ray.color = *(unsigned int *)(hand.addr + l_ray.src.y * hand.line_len
					+ l_ray.src.x * (hand.bpp / 8));
			if ((l_ray.color & 0x00FFFFFF) != 0)
				put_pixel(g, l_ray.draw_start_x + p.x, l_ray.draw_start_y + p.y,
					l_ray.color);
		}
	}
}
