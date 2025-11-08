#include "../../inc/cub3d.h"

/* ---- COLORES ---- */
#define COL_BORDER     0xFFB0B0B0
#define COL_HIGHLIGHT  0xFFFFFFFF
#define COL_FILL       0xFFA00000
#define COL_FILL_MID   0xFF800000
#define COL_FILL_DARK  0xFF500000
#define COL_SHADE      0xFF707070
#define COL_BAR_BG     0xFF202020

/* ---- BORDE RECTANGULAR ---- */
static void	bar_border(t_game *g, int x, int y, int w, int h)
{
	int	i;

	i = 0;
	while (i < w)
	{
		put_pixel(g, x + i, y, COL_BORDER);
		put_pixel(g, x + i, y + h - 1, COL_BORDER);
		i++;
	}
	i = 0;
	while (i < h)
	{
		put_pixel(g, x, y + i, COL_BORDER);
		put_pixel(g, x + w - 1, y + i, COL_BORDER);
		i++;
	}
}

/* ---- RELLENO CON SHADING ADAPTADO A 20px ---- */
static void	bar_fill(t_game *g, int x, int y, int amount)
{
	int	i, j;

	/* highlight */
	i = 1;
	while (i < amount - 1)
	{
		put_pixel(g, x + i, y + 1, COL_HIGHLIGHT);
		i++;
	}
	/* cuerpo rojo (y=2..12) */
	j = 2;
	while (j < 13)
	{
		i = 1;
		while (i < amount - 1)
		{
			put_pixel(g, x + i, y + j, COL_FILL);
			i++;
		}
		j++;
	}
	/* transición (y=13..15) */
	j = 13;
	while (j < 16)
	{
		i = 1;
		while (i < amount - 1)
		{
			put_pixel(g, x + i, y + j, COL_FILL_MID);
			i++;
		}
		j++;
	}
	/* sombra inferior (y=16..17) */
	j = 16;
	while (j < 18)
	{
		i = 1;
		while (i < amount - 1)
		{
			put_pixel(g, x + i, y + j, COL_FILL_DARK);
			i++;
		}
		j++;
	}
}

/* ---- SOMBRA LATERAL DERECHA ---- */
static void	bar_side_shade(t_game *g, int x, int y, int h)
{
	int	j;

	j = 1;
	while (j < h - 1)
	{
		put_pixel(g, x,     y + j, COL_SHADE);
		put_pixel(g, x + 1, y + j, COL_SHADE);
		j++;
	}
}

void	draw_health_bar(t_game *g)
{
	int		w = 200;
	int		h = 20;
	int		x = GAME_WIDTH + 200;
	int		y = 60 + 80;
	double	ratio;
	int		fill;
	int		i, j;

	if (g->player_max_hp <= 0)
		g->player_max_hp = 1;
	ratio = (double)g->player_hp / (double)g->player_max_hp;
	if (ratio < 0.0) ratio = 0.0;
	if (ratio > 1.0) ratio = 1.0;

	fill = (int)(ratio * (double)w);
	if (fill < 0) fill = 0;
	if (fill > w) fill = w;

	/* fondo oscuro */
	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w + 4)
		{
			put_pixel(g, x + j, y + i, COL_BAR_BG);
			j++;
		}
		i++;
	}

	/* barra estilo Spider-Man */
	bar_border(g, x, y, w + 4, h);
	bar_fill(g, x + 2, y, fill);
	bar_side_shade(g, x + fill + 2, y, h);
}

