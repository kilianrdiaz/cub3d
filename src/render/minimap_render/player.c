/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:48:47 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/26 18:48:50 by kroyo-di         ###   ########.fr       */
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

static void	process_triangle_line(t_game *g, t_triangle *t, int y)
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
		if ((t->y[i] <= y && y < t->y[j]) || (t->y[j] <= y && y < t->y[i]))
		{
			ratio = (double)(y - t->y[i]) / (t->y[j] - t->y[i]);
			xhit[hits++] = t->x[i] + ratio * (t->x[j] - t->x[i]);
			if (hits == 2)
				break ;
		}
	}
	if (hits == 2)
		fill_triangle_line(g, y, xhit, t->color);
}

static void	draw_filled_triangle(t_game *g, t_triangle *t)
{
	int	miny;
	int	maxy;
	int	y;

	miny = fmin(t->y[0], fmin(t->y[1], t->y[2]));
	maxy = fmax(t->y[0], fmax(t->y[1], t->y[2]));
	y = miny - 1;
	while (++y <= maxy)
		process_triangle_line(g, t, y);
}

void	draw_player_arrow(t_game *g, int tile, int ox, int oy)
{
	double		ang;
	double		ca;
	double		sa;
	t_triangle	t;
	int			cx;

	ang = atan2(g->spider.dir_y, g->spider.dir_x);
	ca = cos(ang);
	sa = sin(ang);
	cx = ox + (int)(g->spider.x * tile) - (int)round(ca * (tile / 2.0));
	t.x[0] = cx + (int)round(ca * tile);
	t.y[0] = oy + (int)(g->spider.y * tile) - (int)round(sa * (tile / 2.0))
		+ (int)round(sa * tile);
	t.x[1] = cx - (int)round(sa * (tile / 2));
	t.y[1] = oy + (int)(g->spider.y * tile) - (int)round(sa * (tile / 2.0))
		+ (int)round(ca * (tile / 2));
	t.x[2] = cx + (int)round(sa * (tile / 2));
	t.y[2] = oy + (int)(g->spider.y * tile) - (int)round(sa * (tile / 2.0))
		- (int)round(ca * (tile / 2));
	t.color = COL_PLAYER;
	draw_filled_triangle(g, &t);
}
