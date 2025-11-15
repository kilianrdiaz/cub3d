/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_health.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:46:16 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 20:22:36 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

/* ---- BORDE ---- */
static void	bar_border(t_game *g, t_sprite *live)
{
	int	i;

	i = 0;
	while (i < live->width)
	{
		put_pixel(g, live->pos.x + i, live->pos.y, COL_BORDER);
		put_pixel(g, live->pos.x + i, live->pos.y + live->height - 1,
			COL_BORDER);
		i++;
	}
	i = 0;
	while (i < live->height)
	{
		put_pixel(g, live->pos.x, live->pos.y + i, COL_BORDER);
		put_pixel(g, live->pos.x + live->width - 1, live->pos.y + i,
			COL_BORDER);
		i++;
	}
}

/* ---- RELLENO DE UNA LÍNEA ---- */
static void	bar_fill_line(t_game *g, t_sprite *live, int y, int color)
{
	int	i;

	i = 1;
	while (i < live->screen_x - 1)
	{
		put_pixel(g, live->pos.x + i, y, color);
		i++;
	}
}

/* ---- RELLENO COMPLETO (CON PROFUNDIDAD) ---- */
static void	bar_fill(t_game *g, t_sprite *live)
{
	int	y;
	int	fill_height;
	int	color;

	fill_height = live->height - 2;
	y = 1;
	while (y <= fill_height)
	{
		if (y < fill_height * 0.2)
			color = COL_HIGHLIGHT;
		else if (y < fill_height * 0.6)
			color = COL_FILL;
		else if (y < fill_height * 0.85)
			color = COL_FILL_MID;
		else
			color = COL_FILL_DARK;
		bar_fill_line(g, live, live->pos.y + y, color);
		y++;
	}
}

/* ---- FONDO ---- */
static void	bar_bg(t_game *g, t_sprite *live)
{
	int	i;
	int	j;

	i = 0;
	while (i < live->height)
	{
		j = 0;
		while (j < live->width)
		{
			put_pixel(g, live->pos.x + j, live->pos.y + i, COL_BAR_BG);
			j++;
		}
		i++;
	}
}

/* ---- BARRA DE VIDA ---- */
void	draw_health_bar(t_game *g)
{
	t_sprite	live;
	double		ratio;

	/* Tamaño */
	live.width = 300;
	live.height = 30;
	/* Posición */
	live.pos.x = GAME_W + 150;
	live.pos.y = 140;
	ratio = (double)g->lives.player_hp / 100;
	if (ratio < 0.0)
		ratio = 0.0;
	if (ratio > 1.0)
		ratio = 1.0;
	/* Usamos screen_x para almacenar fill */
	live.screen_x = (int)(ratio * (double)live.width);
	if (live.screen_x < 0)
		live.screen_x = 0;
	if (live.screen_x > live.width)
		live.screen_x = live.width;
	/* Dibujar */
	bar_bg(g, &live);
	bar_fill(g, &live);
	bar_border(g, &live);
}
