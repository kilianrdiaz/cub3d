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
		return (g->spider.dirX = 0, g->spider.dirY = -1,
			g->spider.planeX = 0.66, g->spider.planeY = 0, 0);
	if (c == 'S')
		return (g->spider.dirX = 0, g->spider.dirY = 1, g->spider.planeX =
			-0.66, g->spider.planeY = 0, 0);
	if (c == 'E')
		return (g->spider.dirX = 1, g->spider.dirY = 0, g->spider.planeX = 0,
			g->spider.planeY = 0.66, 0);
	if (c == 'W')
		return (g->spider.dirX = -1, g->spider.dirY = 0, g->spider.planeX = 0,
			g->spider.planeY = -0.66, 0);
	if (c == 'P')
		return (g->spider.dirX = -1, g->spider.dirY = 0, g->spider.planeX = 0,
			g->spider.planeY = 0.66, 0);
	return (-1);
}

void	draw_hand(t_game *g)
{
	int		startX;
	int		startY;
	int		color;
	t_pos	p;

	startX = WIDTH / 2 - g->spider.hand.width / 2;
	startY = HEIGHT - g->spider.hand.height; // mano abajo
	p.y = -1;
	while (++p.y < g->spider.hand.height)
	{
		p.x = -1;
		while (++p.x < g->spider.hand.width)
		{
			color = *(unsigned int *)(g->spider.hand.addr + p.y
					* g->spider.hand.line_len + p.x * (g->spider.hand.bpp / 8));
			if ((color & 0x00FFFFFF) != 0) // ignorar fondo transparente
				put_pixel(g, startX + p.x, startY + p.y, color);
		}
	}
}
