/* ************************************************************************** */
/*                                                                            */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                                            */
/*   Función principal del minimapa y constantes globales                     */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void	draw_minimap_elements(t_game *g, t_minimap *m,
					int w, int h, int t)
{
	int				ox;
	int				oy;
	t_sprite_info	inf;

	ox = MINIMAP_OFFSET_X + 6;
	oy = MINIMAP_OFFSET_Y + 6;
	put_rect(g, ox, oy, w * t, h * t, COL_BG);
	draw_map_tiles(g, m, t, ox, oy);
	inf.t = t;
	inf.ox = ox;
	inf.oy = oy;
	inf.color = COL_BOMB;
	draw_sprites_minimap(g, m, g->bombs, &inf);
	inf.color = COL_LIZARD;
	draw_sprites_minimap(g, m, g->lizards, &inf);
	draw_player_arrow(g, t, ox, oy);
}

void	draw_minimap(t_game *g)
{
	t_minimap	*m;
	int			h;
	int			w;
	int			t;

	m = &g->minimap;
	h = 0;
	while (g->map[h])
		h++;
	w = ft_strlen(g->map[0]);
	if (!m->revealed || m->width != w || m->height != h)
		init_revealed_if_needed(m, w, h);
	reveal_radius(m, (int)g->spider.x, (int)g->spider.y, INIT_REVEAL_RADIUS);
	reveal_radius(m, (int)g->spider.x, (int)g->spider.y, REVEAL_STEP_RADIUS);
	t = fmin((MINIMAP_SIZE_LIMIT - 12) / w,
			(MINIMAP_SIZE_LIMIT - 12) / h);
	if (t < 2)
		t = 2;
	draw_minimap_elements(g, m, w, h, t);
}




