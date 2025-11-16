/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:36 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 20:20:03 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void	reveal_radius(t_minimap *m, int cx, int cy, int r)
{
	int	x;
	int	y;
	int	dx;
	int	dy;

	if (!m->revealed)
		return ;
	y = fmax(0, cy - r) - 1;
	while (++y <= fmin(m->height - 1, cy + r))
	{
		x = fmax(0, cx - r) - 1;
		while (++x <= fmin(m->width - 1, cx + r))
		{
			dx = x - cx;
			dy = y - cy;
			if (sqrt(dx * dx + dy * dy) <= r + 0.4)
				m->revealed[y][x] = 1;
		}
	}
}

static void	draw_map_tiles(t_game *g)
{
	int	y;
	int	x;

	y = -1;
	while (g->map[++y])
	{
		x = -1;
		while (g->map[y][++x])
			draw_tile(g, (t_pos){x, y});
	}
}

void	draw_minimap(t_game *g)
{
	int			w;
	int			h;
	t_minimap	*m;

	if (!g->map)
		return ;
	m = &g->minimap;
	h = ft_memlen(g->map);
	w = get_map_max_width(g->map);
	if (!m->revealed || m->width != w || m->height != h)
		init_revealed_if_needed(m, w, h);
	reveal_radius(m, (int)g->spider.pos.x, (int)g->spider.pos.y,
		REVEAL_STEP_RADIUS);
	w *= MINIMAP_TILE;
	h *= MINIMAP_TILE;
	m->offset.x = GAME_W + ((WIDTH - GAME_W) / 2) - (VISIBLE_SIZE / 2);
	m->offset.y = (HEIGHT / 2) - (VISIBLE_SIZE - 100);
	m->cam_x = (int)(g->spider.pos.x * MINIMAP_TILE) - VISIBLE_SIZE / 2;
	m->cam_y = (int)(g->spider.pos.y * MINIMAP_TILE) - VISIBLE_SIZE / 2;
	m->cam_x = clamp_int(m->cam_x, 0, w - VISIBLE_SIZE);
	m->cam_y = clamp_int(m->cam_y, 0, h - VISIBLE_SIZE);
	draw_map_tiles(g);
	draw_sprites_minimap(g);
	draw_player_arrow(g, *m);
}
