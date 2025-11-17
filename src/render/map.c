/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 15:33:01 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/16 15:44:58 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define FADE 3
#define CUTOFF 8

static unsigned int	apply_darkness(unsigned int color, double dist)
{
	unsigned char	rgb[3];
	double			t;
	double			factor;

	if (dist >= CUTOFF)
		return (0x000000);
	if (dist <= FADE)
		return (color);
	t = (dist - FADE) / (CUTOFF - FADE);
	if (t < 0)
		t = 0;
	if (t > 1)
		t = 1;
	factor = 1.0 - t;
	rgb[0] = ((color >> 16) & 0xFF) * factor;
	rgb[1] = ((color >> 8) & 0xFF) * factor;
	rgb[2] = (color & 0xFF) * factor;
	return ((rgb[0] << 16) | (rgb[1] << 8) | rgb[2]);
}

static void	draw_floor_and_ceiling(t_game *g, t_ray *ray, int y)
{
	int				x;
	t_pos			map;
	unsigned int	color;

	x = -1;
	while (++x < GAME_W)
	{
		map.x = (int)ray->side_dist.x;
		map.y = (int)ray->side_dist.y;
		ray->src.x = (int)((ray->side_dist.x - map.x) * g->map_text[F].width);
		ray->src.y = (int)((ray->side_dist.y - map.y) * g->map_text[F].height);
		ray->src.x = clamp_int(ray->src.x, 0, g->map_text[F].width - 1);
		ray->src.y = clamp_int(ray->src.y, 0, g->map_text[F].height - 1);
		color = get_pixel_color(g->map_text[F], ray->src.x, ray->src.y);
		color = apply_darkness(color, ray->row_distance);
		put_pixel(g, x, y, color);
		ray->delta_dist.x = clamp_int(ray->src.x, 0, g->map_text[C].width - 1);
		ray->delta_dist.y = clamp_int(ray->src.y, 0, g->map_text[C].height - 1);
		color = get_pixel_color(g->map_text[C], ray->delta_dist.x,
				ray->delta_dist.y);
		color = apply_darkness(color, ray->row_distance);
		put_pixel(g, x, HEIGHT - y - 1, color);
		ray->side_dist.x += ray->coords.x;
		ray->side_dist.y += ray->coords.y;
	}
}

static void	draw_wall_stripe(t_game *g, t_ray *ray, t_tex tex, int x)
{
	int				d;
	int				y;
	unsigned int	color;

	y = ray->d_start.y - 1;
	d = 0;
	while (++y <= ray->d_end.y)
	{
		d = y * 256 - HEIGHT * 128 + ray->line_height * 128;
		ray->src.y = ((d * tex.height) / ray->line_height) / 256;
		ray->src.y = clamp_int(ray->src.y, 0, tex.height - 1);
		color = get_pixel_color(tex, ray->src.x, ray->src.y);
		color = apply_darkness(color, ray->row_distance);
		put_pixel(g, x, y, color);
	}
}

static void	render_walls(t_game *g)
{
	t_ray	ray;
	t_tex	tex;
	int		side;
	int		x;

	x = -1;
	while (++x < GAME_W)
	{
		ray = ray_map(*g, x);
		if (ray.left.x < 0)
		{
			ray.coords.x = -1;
			ray.side_dist.x = (g->spider.pos.x - ray.src.x) * ray.delta_dist.x;
		}
		if (ray.left.y < 0)
		{
			ray.coords.y = -1;
			ray.side_dist.y = (g->spider.pos.y - ray.src.y) * ray.delta_dist.y;
		}
		calculate_distance_to_wall(*g, &ray, &side);
		tex = get_texture_wall(*g, ray, side);
		calculate_wall_stripe(*g, &ray, tex, side);
		draw_wall_stripe(g, &ray, tex, x);
		g->zbuffer[x] = ray.row_distance;
	}
}

void	render_map(t_game *g)
{
	t_ray	ray;
	int		y;
	int		p;

	if (!g->map_text || !g->map)
		return ;
	y = HEIGHT / 2;
	while (++y < HEIGHT)
	{
		ray.left.x = g->spider.dir.x - g->spider.plane.x;
		ray.left.y = g->spider.dir.y - g->spider.plane.y;
		ray.right.x = g->spider.dir.x + g->spider.plane.x;
		ray.right.y = g->spider.dir.y + g->spider.plane.y;
		p = y - HEIGHT / 2;
		if (p == 0)
			continue ;
		ray.view = 0.5 * HEIGHT;
		ray.row_distance = ray.view / (double)p;
		ray.side_dist.x = g->spider.pos.x + ray.row_distance * ray.left.x;
		ray.side_dist.y = g->spider.pos.y + ray.row_distance * ray.left.y;
		ray.coords.x = ray.row_distance * (ray.right.x - ray.left.x) / GAME_W;
		ray.coords.y = ray.row_distance * (ray.right.y - ray.left.y) / GAME_W;
		draw_floor_and_ceiling(g, &ray, y);
	}
	render_walls(g);
}
