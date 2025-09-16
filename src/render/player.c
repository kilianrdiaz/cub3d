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

static int	set_direction(t_game *g, char c)
{
	if (c == 'N')
		return (g->dirX = 0, g->dirY = -1, g->planeX = 0.66, g->planeY = 0, 0);
	if (c == 'S')
		return (g->dirX = 0, g->dirY = 1, g->planeX = -0.66, g->planeY = 0, 0);
	if (c == 'E')
		return (g->dirX = 1, g->dirY = 0, g->planeX = 0, g->planeY = 0.66, 0);
	if (c == 'W')
		return (g->dirX = -1, g->dirY = 0, g->planeX = 0, g->planeY = -0.66, 0);
	if (c == 'P')
		return (g->dirX = -1, g->dirY = 0, g->planeX = 0, g->planeY = 0.66, 0);
	return (-1);
}

void	draw_hand(t_game *g)
{
	int		startX;
	int		startY;
	int		color;
	t_pos	p;

	startX = WIDTH / 2 - g->hand.width / 2;
	startY = HEIGHT - g->hand.height; // mano abajo
	p.y = -1;
	while (++p.y < g->hand.height)
	{
		p.x = -1;
		while (++p.x < g->hand.width)
		{
			color = *(unsigned int *)(g->hand.addr + p.y * g->hand.line_len
					+ p.x * (g->hand.bpp / 8));
			if ((color & 0x00FFFFFF) != 0) // ignorar fondo transparente
				put_pixel(g, startX + p.x, startY + p.y, color);
		}
	}
}

void	init_player(t_game *g)
{
	t_pos	p;
	char	c;

	p.y = -1;
	while (++p.y < MAP_H)
	{
		p.x = -1;
		while (++p.x < MAP_W)
		{
			c = map[p.y][p.x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'P')
			{
				g->posX = p.x + 0.5;
				g->posY = p.y + 0.5;
				set_direction(g, c);
				return ;
			}
		}
	}
	ft_error_exit("Error: No player start position found in map\n");
}
