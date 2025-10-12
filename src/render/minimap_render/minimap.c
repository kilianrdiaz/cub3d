/* ************************************************************************** */
/*                                                                            */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                                            */
/*   Función principal del minimapa y constantes globales                     */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

/* Variables globales del minimapa */
char **revealed = NULL;
int last_w = 0;
int last_h = 0;

/* === Prototipos === */
void draw_map_tiles(t_game *g, int tile, int ox, int oy, int map_w, int map_h);
void draw_sprites_minimap(t_game *g, t_sprite **arr, int t, int ox, int oy, int color);
void draw_player_arrow(t_game *g, int tile, int ox, int oy);
void init_revealed_if_needed(int w, int h);
void reveal_radius(int cx, int cy, int r, int w, int h);

/* === Principal === */
void draw_minimap(t_game *g)
{
	int h = 0, w, t, ox, oy;

	while (g->map[h])
		h++;
	w = ft_strlen(g->map[0]);

	init_revealed_if_needed(w, h);
	reveal_radius((int)g->spider.x, (int)g->spider.y, INIT_REVEAL_RADIUS, w, h);
	reveal_radius((int)g->spider.x, (int)g->spider.y, REVEAL_STEP_RADIUS, w, h);

	t = fmin((MINIMAP_SIZE_LIMIT - 12) / w, (MINIMAP_SIZE_LIMIT - 12) / h);
	if (t < 2)
		t = 2;

	ox = MINIMAP_OFFSET_X + 6;
	oy = MINIMAP_OFFSET_Y + 6;

	put_rect(g, ox, oy, w * t, h * t, COL_BG);
	draw_map_tiles(g, t, ox, oy, w, h);
	draw_sprites_minimap(g, g->bombs, t, ox, oy, COL_BOMB);
	draw_sprites_minimap(g, g->lizards, t, ox, oy, COL_LIZARD);
	draw_player_arrow(g, t, ox, oy);
}
