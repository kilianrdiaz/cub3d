/* ************************************************************************** */
/*                                                                            */
/*   minimap_player.c                                  :+:      :+:    :+:   */
/*                                                                            */
/*   Dibujo de la flecha del jugador en el minimapa                           */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void draw_filled_triangle(t_game *g,
	int x0, int y0, int x1, int y1, int x2, int y2, int color)
{
	int y, xs, xe, xi;
	double  t;
	int ys[3] = {y0, y1, y2};
	int xs_arr[3] = {x0, x1, x2};
	int miny = fmin(y0, fmin(y1, y2));
	int maxy = fmax(y0, fmax(y1, y2));

	y = miny - 1;
	while (++y <= maxy)
	{
		int hits = 0;
		double xhit[2];
		for (int i = 0; i < 3; ++i)
		{
			int j = (i + 1) % 3;
			if ((ys[i] <= y && y < ys[j]) || (ys[j] <= y && y < ys[i]))
			{
				t = (double)(y - ys[i]) / (ys[j] - ys[i]);
				xhit[hits++] = xs_arr[i] + t * (xs_arr[j] - xs_arr[i]);
				if (hits == 2) break;
			}
		}
		if (hits == 2)
		{
			xs = (int)floor(fmin(xhit[0], xhit[1]));
			xe = (int)ceil(fmax(xhit[0], xhit[1]));
			xi = xs - 1;
			while (++xi <= xe)
				if (xi >= 0 && xi < WIDTH && y >= 0 && y < HEIGHT)
					put_pixel(g, xi, y, color);
		}
	}
}

void draw_player_arrow(t_game *g, int tile, int ox, int oy)
{
	double ang, ca, sa;
	int size, p[6], cx, cy;
	double offset_x, offset_y;

	ang = atan2(g->spider.dir_y, g->spider.dir_x);
	ca = cos(ang);
	sa = sin(ang);
	size = fmax(3, tile);

	cx = ox + (int)(g->spider.x * tile);
	cy = oy + (int)(g->spider.y * tile);

	/* centrado */
	offset_x = -ca * (size / 2.0);
	offset_y = -sa * (size / 2.0);
	cx += (int)round(offset_x);
	cy += (int)round(offset_y);

	p[0] = cx + (int)round(ca * size);
	p[1] = cy + (int)round(sa * size);
	p[2] = cx - (int)round(sa * (size / 2));
	p[3] = cy + (int)round(ca * (size / 2));
	p[4] = cx + (int)round(sa * (size / 2));
	p[5] = cy - (int)round(ca * (size / 2));

	draw_filled_triangle(g, p[0], p[1], p[2], p[3], p[4], p[5], COL_PLAYER);
}
