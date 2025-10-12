/* ************************************************************************** */
/*                                                                            */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                                            */
/*   Versión: minimapa con flecha jugador, bombas/lagartos y fog-of-war.      */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include <stdlib.h>
#include <math.h>

/* Colores (ajusta si tu put_pixel usa otro formato) */
#define COL_BG        0x0B0B0B
#define COL_BORDER    0xBFBFBF
#define COL_WALL      0xD0D0D0
#define COL_FLOOR     0x5A5A5A
#define COL_UNK_FOG   0x151515
#define COL_PLAYER    0xFF2D2D
#define COL_BOMB      0xFFB84D
#define COL_LIZARD    0x5AFF7A

#ifndef MINIMAP_SIZE_LIMIT
# define MINIMAP_SIZE_LIMIT  200
#endif
#ifndef MINIMAP_OFFSET_X
# define MINIMAP_OFFSET_X    (WIDTH - MINIMAP_SIZE_LIMIT - 20)
#endif
#ifndef MINIMAP_OFFSET_Y
# define MINIMAP_OFFSET_Y    20
#endif

/* Fog-of-war settings */
#define INIT_REVEAL_RADIUS 6
#define REVEAL_STEP_RADIUS 3

/* helpers gráficos */
static void put_rect(t_game *g, int x, int y, int w, int h, int color)
{
	int i;
	int j;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			put_pixel(g, x + j, y + i, color);
			++j;
		}
		++i;
	}
}

static void draw_border(t_game *g, int x, int y, int w, int h, int color)
{
	int i;

	if (w <= 0 || h <= 0)
		return ;
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

/* Triángulo relleno (mantener como helper: puede ser largo) */
static void draw_filled_triangle(t_game *g, int x0, int y0, int x1, int y1,
	int x2, int y2, int color)
{
	int miny;
	int maxy;
	int y;

	miny = fmin(y0, fmin(y1, y2));
	maxy = fmax(y0, fmax(y1, y2));
	if (maxy < 0 || miny >= HEIGHT)
		return ;
	y = miny;
	while (y <= maxy)
	{
		double ax = 0;
		double bx = 0;
		int have_a = 0;
		int have_b = 0;
		int xs[3];
		int ys[3];
		int i;

		xs[0] = x0; xs[1] = x1; xs[2] = x2;
		ys[0] = y0; ys[1] = y1; ys[2] = y2;
		i = 0;
		while (i < 3)
		{
			int j = (i + 1) % 3;
			if ((ys[i] <= y && y < ys[j]) || (ys[j] <= y && y < ys[i]))
			{
				double t = (double)(y - ys[i]) / (double)(ys[j] - ys[i]);
				double x = xs[i] + t * (xs[j] - xs[i]);
				if (!have_a) { ax = x; have_a = 1; }
				else { bx = x; have_b = 1; }
			}
			++i;
		}
		if (have_a && have_b)
		{
			int xsx = (int)floor(fmin(ax, bx));
			int xex = (int)ceil(fmax(ax, bx));
			int xi = xsx;
			while (xi <= xex)
			{
				if (xi >= 0 && xi < WIDTH && y >= 0 && y < HEIGHT)
					put_pixel(g, xi, y, color);
				++xi;
			}
		}
		++y;
	}
}

/* Fog-of-war (2D dynamic array) */
static char **revealed = NULL;
static int last_map_w = 0;
static int last_map_h = 0;

static void free_revealed(void)
{
	int i;

	if (!revealed)
		return ;
	i = 0;
	while (i < last_map_h)
	{
		free(revealed[i]);
		++i;
	}
	free(revealed);
	revealed = NULL;
	last_map_w = 0;
	last_map_h = 0;
}

static void init_revealed_if_needed(int map_w, int map_h)
{
	int i;

	if (revealed && map_w == last_map_w && map_h == last_map_h)
		return ;
	if (revealed)
		free_revealed();
	revealed = (char **)malloc(sizeof(char *) * map_h);
	if (!revealed)
		return ;
	i = 0;
	while (i < map_h)
	{
		revealed[i] = (char *)malloc(map_w);
		if (revealed[i])
			ft_memset(revealed[i], 0, map_w);
		++i;
	}
	last_map_w = map_w;
	last_map_h = map_h;
}

static void reveal_radius(int cx, int cy, int radius, int map_w, int map_h)
{
	int y;
	int x;
	int miny;
	int maxy;
	int minx;
	int maxx;

	miny = fmax(0, cy - radius);
	maxy = fmin(map_h - 1, cy + radius);
	minx = fmax(0, cx - radius);
	maxx = fmin(map_w - 1, cx + radius);
	y = miny;
	while (y <= maxy)
	{
		x = minx;
		while (x <= maxx)
		{
			{
				double dx = (double)(x - cx);
				double dy = (double)(y - cy);
				if (sqrt(dx * dx + dy * dy) <= (double)radius + 0.4)
					revealed[y][x] = 1;
			}
			++x;
		}
		++y;
	}
}

static int is_revealed(int mx, int my, int map_w, int map_h)
{
	if (!revealed)
		return (0);
	if (mx < 0 || my < 0 || mx >= map_w || my >= map_h)
		return (0);
	return (revealed[my][mx]);
}

/* dibujo de tiles con fog */
static void draw_map_tiles(t_game *g, int tile, int ox, int oy, int map_w,
	int map_h)
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
				put_rect(g, sx, sy, tile, tile, COL_WALL);
			else
				put_rect(g, sx, sy, tile, tile, COL_FLOOR);
			++x;
		}
		++y;
	}
}

/* dibuja iconos de bombas/lagartos (más grandes) */
static void draw_sprites_on_minimap(t_game *g, int tile, int ox, int oy,
	int map_w, int map_h)
{
	int i;
	t_sprite **arr;
	int sx;
	int sy;
	int icon_size;

	arr = g->bombs;
	if (arr)
	{
		i = 0;
		while (arr[i])
		{
			int mx = (int)arr[i]->x;
			int my = (int)arr[i]->y;
			if (mx >= 0 && my >= 0 && mx < map_w && my < map_h &&
				is_revealed(mx, my, map_w, map_h))
			{
				sx = ox + (int)(arr[i]->x * tile);
				sy = oy + (int)(arr[i]->y * tile);
				icon_size = fmax(2, tile / 2); /* bombas más grandes */
				put_rect(g, sx - icon_size / 2, sy - icon_size / 2,
					icon_size, icon_size, COL_BOMB);
			}
			++i;
		}
	}
	arr = g->lizards;
	if (arr)
	{
		i = 0;
		while (arr[i])
		{
			int mx = (int)arr[i]->x;
			int my = (int)arr[i]->y;
			if (mx >= 0 && my >= 0 && mx < map_w && my < map_h &&
				is_revealed(mx, my, map_w, map_h))
			{
				sx = ox + (int)(arr[i]->x * tile);
				sy = oy + (int)(arr[i]->y * tile);
				icon_size = fmax(1, tile / 2); /* lagartos más grandes */
				put_rect(g, sx - icon_size / 2, sy - icon_size / 2,
					icon_size, icon_size, COL_LIZARD);
			}
			++i;
		}
	}
}

/* dibuja la flecha-jugador (FLECHA MÁS GRANDE). Sin rayo azul. */
static void draw_player_arrow(t_game *g, int tile, int ox, int oy)
{
	double ang;
	double ca;
	double sa;
	int size;
	int p[6];
	int cx;
	int cy;

	ang = atan2(g->spider.dir_y, g->spider.dir_x);
	ca = cos(ang);
	sa = sin(ang);
	size = fmax(3, tile); /* flecha más grande: al menos tile píxeles */
	cx = ox + (int)(g->spider.x * tile);
	cy = oy + (int)(g->spider.y * tile);
	/* puntos del triángulo en array p = {x0,y0,x1,y1,x2,y2} */
	p[0] = cx + (int)round(ca * size);
	p[1] = cy + (int)round(sa * size);
	p[2] = cx - (int)round(sa * (size / 2));
	p[3] = cy + (int)round(ca * (size / 2));
	p[4] = cx + (int)round(sa * (size / 2));
	p[5] = cy - (int)round(ca * (size / 2));
	draw_filled_triangle(g, p[0], p[1], p[2], p[3], p[4], p[5], COL_PLAYER);
}

/* entrada principal del minimapa */
void draw_minimap(t_game *g)
{
	int map_h;
	int map_w;
	int tile;
	double scale_x;
	double scale_y;
	int max_size;
	int pad;
	int container_w;
	int container_h;
	int ox;
	int oy;

	if (!g || !g->map || !g->map[0])
		return ;
	map_h = 0;
	while (g->map[map_h])
		++map_h;
	map_w = ft_strlen(g->map[0]);
	if (map_w <= 0 || map_h <= 0)
		return ;
	init_revealed_if_needed(map_w, map_h);
	{
		int px_cell = (int)g->spider.x;
		int py_cell = (int)g->spider.y;
		int any_revealed = 0;
		int i;

		i = 0;
		while (i < map_h && !any_revealed)
		{
			int j = 0;
			while (j < map_w && !any_revealed)
			{
				if (revealed[i][j])
					any_revealed = 1;
				++j;
			}
			++i;
		}
		if (!any_revealed)
			reveal_radius(px_cell, py_cell, INIT_REVEAL_RADIUS, map_w, map_h);
	}
	{
		int px_cell = (int)g->spider.x;
		int py_cell = (int)g->spider.y;
		reveal_radius(px_cell, py_cell, REVEAL_STEP_RADIUS, map_w, map_h);
	}
	max_size = MINIMAP_SIZE_LIMIT;
	pad = 6;
	scale_x = (double)(max_size - pad * 2) / (double)map_w;
	scale_y = (double)(max_size - pad * 2) / (double)map_h;
	tile = (int)floor(fmin(scale_x, scale_y));
	if (tile < 2)
		tile = 2;
	if (tile > MINIMAP_SCALE * 2)
		tile = MINIMAP_SCALE * 2;
	container_w = map_w * tile + pad * 2;
	container_h = map_h * tile + pad * 2;
	ox = MINIMAP_OFFSET_X + (MINIMAP_SIZE_LIMIT - container_w) / 2;
	oy = MINIMAP_OFFSET_Y;
	put_rect(g, ox, oy, container_w, container_h, COL_BG);
	draw_border(g, ox, oy, container_w, container_h, COL_BORDER);
	draw_map_tiles(g, tile, ox + pad, oy + pad, map_w, map_h);
	draw_sprites_on_minimap(g, tile, ox + pad, oy + pad, map_w, map_h);
	draw_player_arrow(g, tile, ox + pad, oy + pad);
}
