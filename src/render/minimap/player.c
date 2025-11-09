/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:48:47 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/09 16:07:19 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void	fill_triangle_line(t_game *g, int y, double xh[2])
{
	int	x;
	int	xs;
	int	xe;

	xs = (int)floor(fmin(xh[0], xh[1]));
	xe = (int)ceil(fmax(xh[0], xh[1]));
	x = xs - 1;
	while (++x <= xe)
	{
		if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
			put_pixel(g, x, y, COL_PLAYER);
	}
}

static void	process_triangle_line(t_game *g, t_pos *t, int y)
{
	double	r;
	double	xh[2];
	int		h;
	int		i;
	int		j;

	h = 0;
	i = -1;
	while (++i < 3)
	{
		j = (i + 1) % 3;
		if ((t[i].y <= y && y < t[j].y) || (t[j].y <= y && y < t[i].y))
		{
			r = (double)(y - t[i].y) / (t[j].y - t[i].y);
			xh[h++] = t[i].x + r * (t[j].x - t[i].x);
			if (h == 2)
				break ;
		}
	}
	if (h == 2)
		fill_triangle_line(g, y, xh);
}

static void	draw_triangle(t_game *g, t_pos *t)
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

static void	set_arrow_points(t_pos *t, t_game *g, t_minimap *m, double ts)
{
	double	ang;
	double	ca;
	double	sa;
	int		xc;

	ang = atan2(g->spider.dir.y, g->spider.dir.x);
	ca = cos(ang);
	sa = sin(ang);
	xc = m->offset.x + (int)(g->spider.pos.x * ts) - m->cam_x - (int)round(ca
			* (ts / 2.0));
	t[0].x = xc + (int)round(ca * ts);
	t[0].y = m->offset.y + (int)(g->spider.pos.y * ts) - m->cam_y
		- (int)round(sa * (ts / 2.0)) + (int)round(sa * ts);
	t[1].x = xc - (int)round(sa * (ts / 2.0));
	t[1].y = m->offset.y + (int)(g->spider.pos.y * ts) - m->cam_y
		- (int)round(sa * (ts / 2.0)) + (int)round(ca * (ts / 2.0));
	t[2].x = xc + (int)round(sa * (ts / 2.0));
	t[2].y = m->offset.y + (int)(g->spider.pos.y * ts) - m->cam_y
		- (int)round(sa * (ts / 2.0)) - (int)round(ca * (ts / 2.0));
}

void	draw_player_arrow(t_game *g, t_minimap m)
{
	t_pos	p[3];
	double	ts;

	ts = m.tile_size;
	set_arrow_points(p, g, &m, ts);
	draw_triangle(g, p);
}
