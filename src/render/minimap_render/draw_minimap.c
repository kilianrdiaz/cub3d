/* ************************************************************************** */
/*                                                                            */
/*   minimap_draw.c                                    :+:      :+:    :+:   */
/*                                                                            */
/*   Dibujo del minimapa: bordes, rectángulos y tiles del mapa                */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

/* === Bordes y rectángulos === */
void draw_border(t_game *g, int x, int y, int w, int h, int color)
{
	int i;

	if (w <= 1 || h <= 1)
		return;
	i = 0;
	while (i < w)
	{
		put_pixel(g, x + i, y, color);
		put_pixel(g, x + i, y + h - 1, color);
		++i;
	}
	i = 0;
	while (i < h)
	{
		put_pixel(g, x, y + i, color);
		put_pixel(g, x + w - 1, y + i, color);
		++i;
	}
}

void put_rect(t_game *g, int x, int y, int w, int h, int c)
{
	int i;
	int j;

	i = -1;
	while (++i < h)
	{
		j = -1;
		while (++j < w)
			put_pixel(g, x + j, y + i, c);
	}
}

/* === Dibujo de tiles === */

extern int is_revealed(int x, int y, int w, int h);

void draw_map_tiles(t_game *g, int tile, int ox, int oy, int map_w, int map_h)
{
	int y;
	int x;
	int sx;
	int sy;

	y = 0;
	while (g->map[y])
	{
		x = 0;
		while (g->map[y][x])
		{
			sx = ox + x * tile;
			sy = oy + y * tile;
			if (!is_revealed(x, y, map_w, map_h))
				put_rect(g, sx, sy, tile, tile, COL_UNK_FOG);
			else if (g->map[y][x] == '1')
			{
				put_rect(g, sx, sy, tile, tile, COL_WALL);
				draw_border(g, sx, sy, tile, tile, COL_WALL_BORDER);
			}
			else
				put_rect(g, sx, sy, tile, tile, 0x000000);
			++x;
		}
		++y;
	}
}
