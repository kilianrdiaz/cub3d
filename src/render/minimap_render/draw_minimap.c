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

void	put_rect(t_game *g, t_rect r)
{
	int	i;
	int	j;

	if (r.w <= 0 || r.h <= 0)
		return ;
	i = -1;
	while (++i < r.h)
	{
		j = -1;
		while (++j < r.w)
			put_pixel(g, r.x + j, r.y + i, r.c);
	}
}

void	draw_border(t_game *g, t_rect r)
{
	int	i;

	if (r.w <= 1 || r.h <= 1)
		return ;
	i = -1;
	while (++i < r.w)
	{
		put_pixel(g, r.x + i, r.y, r.c);
		put_pixel(g, r.x + i, r.y + r.h - 1, r.c);
	}
	i = -1;
	while (++i < r.h)
	{
		put_pixel(g, r.x, r.y + i, r.c);
		put_pixel(g, r.x + r.w - 1, r.y + i, r.c);
	}
}

static void	draw_wall_tile(t_game *g, t_rect r)
{
	r.c = COL_WALL;
	put_rect(g, r);
	r.c = COL_WALL_BORDER;
	draw_border(g, r);
}

static void	draw_basic_tile(t_game *g, int color, t_rect r)
{
	r.c = color;
	put_rect(g, r);
}

void	draw_tile(t_game *g, t_minimap *m, t_tile tile)
{
	t_rect	r;

	r.x = tile.info.ox + tile.x * tile.info.t;
	r.y = tile.info.oy + tile.y * tile.info.t;
	r.w = tile.info.t;
	r.h = tile.info.t;
	if (!m->revealed || !m->revealed[tile.y][tile.x])
		draw_basic_tile(g, COL_UNK_FOG, r);
	else if (g->map[tile.y][tile.x] == '1')
		draw_wall_tile(g, r);
	else
		draw_basic_tile(g, 0x000000, r);
}
