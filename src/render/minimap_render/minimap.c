/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:36 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/29 21:47:22 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

void	draw_map_tiles(t_game *g, t_minimap *m, t_tileinfo info)
{
	int		y;
	int		x;
	t_tile	tile;

	y = -1;
	while (g->map[++y])
	{
		x = -1;
		while (g->map[y][++x])
		{
			tile.x = x;
			tile.y = y;
			tile.info = info;
			draw_tile(g, m, tile);
		}
	}
}

static void	draw_minimap_elements(t_game *g, t_minimap *m, t_tileinfo info,
		t_dims dims)
{
	int				ox;
	int				oy;
	t_sprite_info	inf;
	t_rect			bg;

	ox = MINIMAP_OFFSET_X + 6;
	oy = MINIMAP_OFFSET_Y + 6;
	bg.x = ox;
	bg.y = oy;
	bg.w = dims.w * info.t;
	bg.h = dims.h * info.t;
	bg.c = COL_BG;
	put_rect(g, bg);
	draw_map_tiles(g, m, info);
	inf.t = info.t;
	inf.ox = ox;
	inf.oy = oy;
	inf.color = COL_BOMB;
	draw_sprites_minimap(g, m, g->bombs, &inf);
	inf.color = COL_LIZARD;
	draw_sprites_minimap(g, m, g->lizards, &inf);
	draw_player_arrow(g, info.t, ox, oy);
}

void	draw_minimap(t_game *g)
{
	t_minimap	*m;
	int			h;
	int			w;
	int			t;
	t_tileinfo	info;

	m = &g->minimap;
	h = 0;
	while (g->map[h])
		h++;
	w = ft_strlen(g->map[0]);
	if (!m->revealed || m->width != w || m->height != h)
		init_revealed_if_needed(m, w, h);
	reveal_radius(m, (int)g->spider.pos.x, (int)g->spider.pos.y, INIT_REVEAL_RADIUS);
	reveal_radius(m, (int)g->spider.pos.x, (int)g->spider.pos.y, REVEAL_STEP_RADIUS);
	t = fmin((MINIMAP_SIZE_LIMIT - 12) / w, (MINIMAP_SIZE_LIMIT - 12) / h);
	if (t < 2)
		t = 2;
	info.t = t;
	info.ox = MINIMAP_OFFSET_X + 6;
	info.oy = MINIMAP_OFFSET_Y + 6;
	draw_minimap_elements(g, m, info, (t_dims){w, h});
}
