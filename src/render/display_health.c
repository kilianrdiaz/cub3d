/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_health.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:46:16 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/09 16:52:16 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define COL_BORDER     0xFFB0B0B0
#define COL_HIGHLIGHT  0xFFFFFFFF
#define COL_FILL       0xFFA00000
#define COL_FILL_MID   0xFF800000
#define COL_FILL_DARK  0xFF500000
#define COL_SHADE      0xFF707070
#define COL_BAR_BG     0xFF202020

/* ---- BORDE ---- */
static void	bar_border(t_game *g, t_bar *b)
{
	int	i;

	i = 0;
	while (i < b->w)
	{
		put_pixel(g, b->x + i, b->y, COL_BORDER);
		put_pixel(g, b->x + i, b->y + b->h - 1, COL_BORDER);
		i++;
	}
	i = 0;
	while (i < b->h)
	{
		put_pixel(g, b->x, b->y + i, COL_BORDER);
		put_pixel(g, b->x + b->w - 1, b->y + i, COL_BORDER);
		i++;
	}
}

/* ---- RELLENO DE UNA LÍNEA ---- */
static void	bar_fill_line(t_game *g, t_bar *b, int color)
{
	int	i;

	i = 1;
	while (i < b->fill - 1)
	{
		put_pixel(g, b->x + i, b->y, color);
		i++;
	}
}

/* ---- RELLENO COMPLETO ---- */
static void	bar_fill(t_game *g, t_bar *b)
{
	int	j;

	b->y += 1;
	bar_fill_line(g, b, COL_HIGHLIGHT);
	j = 1;
	while (++j < 13)
	{
		b->y = b->y + 1;
		bar_fill_line(g, b, COL_FILL);
	}
	j = 0;
	while (j++ < 3)
	{
		b->y = b->y + 1;
		bar_fill_line(g, b, COL_FILL_MID);
	}
	j = 0;
	while (j++ < 2)
	{
		b->y = b->y + 1;
		bar_fill_line(g, b, COL_FILL_DARK);
	}
}

/* ---- SOMBRA ---- */
static void	bar_side_shade(t_game *g, t_bar *b)
{
	int	j;

	j = 1;
	while (j < b->h - 1)
	{
		put_pixel(g, b->x, b->y + j, COL_SHADE);
		put_pixel(g, b->x + 1, b->y + j, COL_SHADE);
		j++;
	}
}

/* ---- FONDO ---- */
static void	bar_bg(t_game *g, t_bar *b)
{
	int	i;
	int	j;

	i = 0;
	while (i < b->h)
	{
		j = 0;
		while (j < b->w + 4)
		{
			put_pixel(g, b->x + j, b->y + i, COL_BAR_BG);
			j++;
		}
		i++;
	}
}

/* ---- BARRA DE VIDA ---- */
void	draw_health_bar(t_game *g)
{
	t_bar	b;
	double	ratio;

	b.w = 200;
	b.h = 20;
	b.x = GAME_WIDTH + 200;
	b.y = 140;
	if (g->player_max_hp <= 0)
		g->player_max_hp = 1;
	ratio = (double)g->player_hp / (double)g->player_max_hp;
	if (ratio < 0.0)
		ratio = 0.0;
	if (ratio > 1.0)
		ratio = 1.0;
	b.fill = (int)(ratio * (double)b.w);
	if (b.fill < 0)
		b.fill = 0;
	if (b.fill > b.w)
		b.fill = b.w;
	bar_bg(g, &b);
	bar_border(g, &b);
	bar_fill(g, &b);
	b.x += b.fill + 2;
	bar_side_shade(g, &b);
}
