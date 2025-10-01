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
	int		color;
	t_pos	p;
	t_tex	hand;

	if (!g->spider.hand || !g->spider.hand[0].addr || !g->spider.hand[1].addr)
		return ;
	hand = g->spider.hand[0];
	if (g->spider.state == ATTACKING)
		hand = g->spider.hand_attack;
	ray.draw_start_x = WIDTH / 2 - hand.width / 2;
	ray.draw_start_y = HEIGHT - hand.height;
	p.y = -1;
	while (++p.y < hand.height)
	{
		p.x = -1;
		while (++p.x < hand.width)
		{
			color = *(unsigned int *)(hand.addr + p.y * hand.line_len + p.x
					* (hand.bpp / 8));
			if ((color & 0x00FFFFFF) != 0) // ignorar fondo transparente
				put_pixel(g, ray.draw_start_x + p.x, ray.draw_start_y + p.y,
					color);
		}
	}
}
