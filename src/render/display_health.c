/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_health.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:46:16 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/09 19:43:53 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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
static void	bar_fill_line(t_game *g, t_bar *b, int y, int color)
{
	int	i;

	i = 1;
	while (i < b->fill - 1)
	{
		put_pixel(g, b->x + i, y, color);
		i++;
	}
}

/* ---- RELLENO COMPLETO (CON PROFUNDIDAD) ---- */
static void	bar_fill(t_game *g, t_bar *b)
{
	int	y;
	int	fill_height;

	/* Controlar proporciones relativas */
	fill_height = b->h - 2;
	y = 1;
	while (y <= fill_height)
	{
		int color;

		if (y < fill_height * 0.2)
			color = COL_HIGHLIGHT; /* Parte superior brillante */
		else if (y < fill_height * 0.6)
			color = COL_FILL; /* Zona media */
		else if (y < fill_height * 0.85)
			color = COL_FILL_MID; /* Parte inferior más oscura */
		else
			color = COL_FILL_DARK; /* Sombra final */
		bar_fill_line(g, b, b->y + y, color);
		y++;
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
		while (j < b->w)
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

	b.w = 300;
	b.h = 30; /* Puedes cambiar libremente la altura ahora */
	b.x = GAME_WIDTH + 150;
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
	bar_fill(g, &b);
	bar_border(g, &b);
}
