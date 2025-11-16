/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:00:05 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 20:20:09 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static int	is_outside(int sx, int sy, t_minimap *m, int ts)
{
	if (sx + ts < m->offset.x || sx > m->offset.x + VISIBLE_SIZE)
		return (1);
	if (sy + ts < m->offset.y || sy > m->offset.y + VISIBLE_SIZE)
		return (1);
	return (0);
}

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

void	draw_tile(t_game *g, t_pos t)
{
	int			sx;
	int			sy;
	t_sprite	r;
	t_minimap	*m;

	m = &g->minimap;
	r.width = MINIMAP_TILE;
	r.height = MINIMAP_TILE;
	sx = m->offset.x + t.x * MINIMAP_TILE - m->cam_x;
	sy = m->offset.y + t.y * MINIMAP_TILE - m->cam_y;
	if (is_outside(sx, sy, m, MINIMAP_TILE))
		return ;
	r.pos.x = sx;
	r.pos.y = sy;
	if (!m->revealed || !m->revealed[t.y][t.x])
		put_rect(g, r, COL_UNK_FOG);
	else if (g->map[t.y][t.x] == '1')
	{
		put_rect(g, r, COL_WALL);
		draw_border(g, r, COL_WALL_BORDER);
	}
	else
		put_rect(g, r, COL_FLOOR);
}
