/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:36 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 17:04:20 by kroyo-di         ###   ########.fr       */
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

static void	clamp_camera(t_minimap *m)
{
	if (m->cam_x < 0)
		m->cam_x = 0;
	if (m->cam_y < 0)
		m->cam_y = 0;
	if (m->cam_x > m->real_w - m->visible_size)
		m->cam_x = m->real_w - m->visible_size;
	if (m->cam_y > m->real_h - m->visible_size)
		m->cam_y = m->real_h - m->visible_size;
}

void	draw_minimap(t_game *g)
{
	int			w;
	int			h;
	int			bar_w;
	t_minimap	*m;

	m = &g->minimap;
	h = ft_memlen(g->map);
	w = get_map_max_width(g->map);
	if (!m->revealed || m->width != w || m->height != h)
		init_revealed_if_needed(m, w, h);
	reveal_radius(m, (int)g->spider.pos.x, (int)g->spider.pos.y,
		REVEAL_STEP_RADIUS);
	m->tile_size = MINIMAP_TILE;
	m->real_w = w * m->tile_size;
	m->real_h = h * m->tile_size;
	bar_w = WIDTH - GAME_WIDTH;
	m->visible_size = 300;
	m->offset.x = GAME_WIDTH + (bar_w / 2) - (m->visible_size / 2);
	m->offset.y = (HEIGHT / 2) - (m->visible_size - 100);
	m->cam_x = (int)(g->spider.pos.x * m->tile_size) - m->visible_size / 2;
	m->cam_y = (int)(g->spider.pos.y * m->tile_size) - m->visible_size / 2;
	clamp_camera(m);
	draw_map_tiles(g);
	draw_sprites_minimap(g);
	draw_player_arrow(g, *m);
}
