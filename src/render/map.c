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

static void	draw_floor_and_ceiling(t_game *g, t_ray *ray, int y)
{
	int		x;
	t_pos	map;

	x = -1;
	while (++x < GAME_WIDTH)
	{
		map.x = (int)ray->side_dist_x;
		map.y = (int)ray->side_dist_y;
		ray->src.x = (int)((ray->side_dist_x - map.x) * g->map_text[F].width);
		ray->src.y = (int)((ray->side_dist_y - map.y) * g->map_text[F].height);
		/* clamp por si hay rounding negativo/extraño */
		ray->src.x = clamp_int(ray->src.x, 0, g->map_text[F].width - 1);
		ray->src.y = clamp_int(ray->src.y, 0, g->map_text[F].height - 1);
		ray->color = *(unsigned int *)(g->map_text[F].addr + ray->src.y
				* g->map_text[F].line_len + ray->src.x * (g->map_text[F].bpp
					/ 8));
		put_pixel(g, x, y, ray->color);
		/* techo espejo */
		ray->delta_dist_x = clamp_int(ray->src.x, 0, g->map_text[C].width - 1);
		ray->delta_dist_y = clamp_int(ray->src.y, 0, g->map_text[C].height - 1);
		ray->color = *(unsigned int *)(g->map_text[C].addr
				+ (int)ray->delta_dist_y * g->map_text[C].line_len
				+ (int)ray->delta_dist_x * (g->map_text[C].bpp / 8));
		put_pixel(g, x, HEIGHT - y - 1, ray->color);
		ray->side_dist_x += ray->coords.x;
		ray->side_dist_y += ray->coords.y;
	}
}

static void	draw_wall_stripe(t_game *g, t_ray *ray, t_tex tex, int x)
{
	int	d;
	int	y;

	y = ray->d_start.y - 1;
	d = 0;
	while (++y <= ray->d_end.y)
	{
		d = y * 256 - HEIGHT * 128 + ray->line_height * 128;
		ray->src.y = ((d * tex.height) / ray->line_height) / 256;
		ray->src.y = clamp_int(ray->src.y, 0, tex.height - 1);
		ray->color = *(unsigned int *)(tex.addr + ray->src.y * tex.line_len
				+ ray->src.x * (tex.bpp / 8));
		put_pixel(g, x, y, ray->color);
	}
}

static void	calculate_distance_to_wall(t_game g, t_ray *ray, int *side)
{
	while (ray->src.x >= 0 && ray->src.y >= 0 && g.map[ray->src.y]
		&& g.map[ray->src.y][ray->src.x] != '1')
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->src.x += ray->coords.x;
			*side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->src.y += ray->coords.y;
			*side = 1;
		}
	}
	// 4. Cálculo de la distancia perpendicular a la pared
	ray->row_distance = (ray->src.y - g.spider.pos.y + (1 - ray->coords.y)
			/ 2.0) / ray->left.y;
	if (!(*side))
		ray->row_distance = (ray->src.x - g.spider.pos.x + (1 - ray->coords.x)
				/ 2.0) / ray->left.x;
	if (ray->row_distance <= 0.0)
		ray->row_distance = 1e-6; // Evita divisiones por 0
}

void	render_wall(t_game *g)
{
	t_ray	ray;
	t_tex	tex;
	int		side;
	int		x;

	x = -1;
	while (++x < GAME_WIDTH)
	{
		ray = ray_map(*g, x);
		if (ray.left.x < 0)
		{
			ray.coords.x = -1;
			ray.side_dist_x = (g->spider.pos.x - ray.src.x) * ray.delta_dist_x;
		}
		if (ray.left.y < 0)
		{
			ray.coords.y = -1;
			ray.side_dist_y = (g->spider.pos.y - ray.src.y) * ray.delta_dist_y;
		}
		calculate_distance_to_wall(*g, &ray, &side);
		tex = get_texture_wall(*g, ray, side);
		calculate_wall_stripe(*g, &ray, tex, side);
		draw_wall_stripe(g, &ray, tex, x);
		g->zbuffer[x] = ray.row_distance; // Guardamos la distancia del rayo
	}
}

void	render_floor_and_ceiling(t_game *g)
{
	t_ray	ray;
	int		y;
	int		p;

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
		ray.side_dist_x = g->spider.pos.x + ray.row_distance * ray.left.x;
		ray.side_dist_y = g->spider.pos.y + ray.row_distance * ray.left.y;
		ray.coords.x = ray.row_distance * (ray.right.x - ray.left.x)
			/ (double)GAME_WIDTH;
		ray.coords.y = ray.row_distance * (ray.right.y - ray.left.y)
			/ (double)GAME_WIDTH;
		draw_floor_and_ceiling(g, &ray, y);
	}
}
