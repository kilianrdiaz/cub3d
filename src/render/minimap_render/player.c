/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:48:47 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/29 21:38:44 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void	fill_triangle_line(t_game *g, int y, double xhit[2], int color)
{
	int	xs;
	int	xe;
	int	xi;

	xs = (int)floor(fmin(xhit[0], xhit[1]));
	xe = (int)ceil(fmax(xhit[0], xhit[1]));
	xi = xs - 1;
	while (++xi <= xe)
		if (xi >= 0 && xi < WIDTH && y >= 0 && y < HEIGHT)
			put_pixel(g, xi, y, color);
}

static void	process_triangle_line(t_game *g, t_pos *t, int y)
{
	int		i;
	int		hits;
	double	ratio;
	double	xhit[2];
	int		j;

	hits = 0;
	i = -1;
	while (++i < 3)
	{
		j = (i + 1) % 3;
		if ((t[i].y <= y && y < t[j].y) || (t[j].y <= y && y < t[i].y))
		{
			ratio = (double)(y - t[i].y) / (t[j].y - t[i].y);
			xhit[hits++] = t[i].x + ratio * (t[j].x - t[i].x);
			if (hits == 2)
				break ;
		}
	}
	if (hits == 2)
		fill_triangle_line(g, y, xhit, COL_PLAYER);
}

static void	draw_filled_triangle(t_game *g, t_pos *t)
{
	int	miny;
	int	maxy;
	int	y;

	miny = fmin(t[0].y, fmin(t[1].y, t[2].y));
	maxy = fmax(t[0].y, fmax(t[1].y, t[2].y));
	y = miny - 1;
	while (++y <= maxy)
		process_triangle_line(g, t, y);
}

void	draw_player_arrow(t_game *g, int tile, int ox, int oy)
{
	double		ang;
	double		ca;
	double		sa;
	t_pos		t[3];
	int			cx;

	ang = atan2(g->spider.dir.y, g->spider.dir.x);
	ca = cos(ang);
	sa = sin(ang);
	cx = ox + (int)(g->spider.pos.x * tile) - (int)round(ca * (tile / 2.0));
	t[0].x = cx + (int)round(ca * tile);
	t[0].y = oy + (int)(g->spider.pos.y * tile) - (int)round(sa * (tile / 2.0))
		+ (int)round(sa * tile);
	t[1].x = cx - (int)round(sa * (tile / 2));
	t[1].y = oy + (int)(g->spider.pos.y * tile) - (int)round(sa * (tile / 2.0))
		+ (int)round(ca * (tile / 2));
	t[2].x = cx + (int)round(sa * (tile / 2));
	t[2].y = oy + (int)(g->spider.pos.y * tile) - (int)round(sa * (tile / 2.0))
		- (int)round(ca * (tile / 2));
	draw_filled_triangle(g, t);
}
