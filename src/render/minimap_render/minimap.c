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

void	draw_map_tiles(t_game *g)
{
	t_pos	pos;

	pos.y = -1;
	while (g->map[++pos.y])
	{
		pos.x = -1;
		while (g->map[pos.y][++pos.x])
			draw_tile(g, pos);
	}
}

void	draw_minimap(t_game *g)
{
	int	h;
	int	w;

	h = ft_memlen(g->map);
	w = ft_strlen(g->map[0]);
	if (!g->minimap.revealed || g->minimap.width != w || g->minimap.height != h)
		init_revealed_if_needed(&g->minimap, w, h);
	reveal_radius(&g->minimap, (int)g->spider.pos.x, (int)g->spider.pos.y,
		REVEAL_STEP_RADIUS);
	g->minimap.tile_size = fmin((MINIMAP_SIZE_LIMIT - 12) / w,
			(MINIMAP_SIZE_LIMIT - 12) / h);
	if (g->minimap.tile_size < 2)
		g->minimap.tile_size = 2;
	g->minimap.offset.x = MINIMAP_OFFSET_X + 6;
	g->minimap.offset.y = MINIMAP_OFFSET_Y + 6;
	draw_map_tiles(g);
	draw_sprites_minimap(g);
	draw_player_arrow(g, g->minimap);
}
