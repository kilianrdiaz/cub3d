/* ************************************************************************** */
/*                                                                            */
/*   minimap_draw.c                                    :+:      :+:    :+:   */
/*                                                                            */
/*   Dibujo del minimapa: bordes, rectángulos y tiles del mapa                */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

void	draw_border(t_game *g, int x, int y, int w, int h, int color)
{
	int	i;

	if (w <= 1 || h <= 1)
		return ;
	i = -1;
	while (++i < w)
	{
		put_pixel(g, x + i, y, color);
		put_pixel(g, x + i, y + h - 1, color);
	}
	i = -1;
	while (++i < h)
	{
		put_pixel(g, x, y + i, color);
		put_pixel(g, x + w - 1, y + i, color);
	}
}

void	put_rect(t_game *g, int x, int y, int w, int h, int c)
{
	int	i;
	int	j;

	i = -1;
	while (++i < h)
	{
		j = -1;
		while (++j < w)
			put_pixel(g, x + j, y + i, c);
	}
}

static void	draw_tile(t_game *g, t_minimap *m, int x, int y, int t, int ox, int oy)
{
	int	sx;
	int	sy;

	sx = ox + x * t;
	sy = oy + y * t;
	if (!m->revealed || !m->revealed[y][x])
		put_rect(g, sx, sy, t, t, COL_UNK_FOG);
	else if (g->map[y][x] == '1')
	{
		put_rect(g, sx, sy, t, t, COL_WALL);
		draw_border(g, sx, sy, t, t, COL_WALL_BORDER);
	}
	else
		put_rect(g, sx, sy, t, t, 0x000000);
}

void	draw_map_tiles(t_game *g, t_minimap *m, int t, int ox, int oy)
{
	int	y;
	int	x;

	y = -1;
	while (g->map[++y])
	{
		x = -1;
		while (g->map[y][++x])
			draw_tile(g, m, x, y, t, ox, oy);
	}
}



