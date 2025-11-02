/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:07 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/29 21:47:53 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

void	put_rect(t_game *g, t_sprite r, unsigned int color)
{
	int	i;
	int	j;

	if (r.width <= 0 || r.height <= 0)
		return ;
	i = -1;
	while (++i < r.height)
	{
		j = -1;
		while (++j < r.width)
			put_pixel(g, r.pos.x + j, r.pos.y + i, color);
	}
}

void	draw_border(t_game *g, t_sprite r, unsigned int color)
{
	int	i;

	if (r.width <= 1 || r.height <= 1)
		return ;
	i = -1;
	while (++i < r.width)
	{
		put_pixel(g, r.pos.x + i, r.pos.y, color);
		put_pixel(g, r.pos.x + i, r.pos.y + r.height - 1, color);
	}
	i = -1;
	while (++i < r.height)
	{
		put_pixel(g, r.pos.x, r.pos.y + i, color);
		put_pixel(g, r.pos.x + r.width - 1, r.pos.y + i, color);
	}
}

void	draw_tile(t_game *g, t_pos tile)
{
	t_sprite	r;

	r.pos.x = g->minimap.offset.x + tile.x * g->minimap.tile_size;
	r.pos.y = g->minimap.offset.y + tile.y * g->minimap.tile_size;
	r.width = g->minimap.tile_size;
	r.height = g->minimap.tile_size;
	if (!g->minimap.revealed || !g->minimap.revealed[tile.y][tile.x])
		put_rect(g, r, COL_UNK_FOG);
	else if (g->map[tile.y][tile.x] == '1')
	{
		put_rect(g, r, COL_WALL);
		draw_border(g, r, COL_WALL_BORDER);
	}
	else
		put_rect(g, r, COL_FLOOR);
}
