/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:45:16 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 14:45:19 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	draw_floor_and_ceiling(t_game *g, t_ray *ray, int y)
{
	int	x;

	x = -1;
	while (++x < GAME_WIDTH)
	{
		ray->map_x = (int)ray->side_dist_x;
		ray->map_y = (int)ray->side_dist_y;
		/* seguridad por si las texturas tienen tamaño 0 (ya comprobado al cargar) */
		ray->tx = (int)((ray->side_dist_x - ray->map_x) * g->floor.width);
		ray->ty = (int)((ray->side_dist_y - ray->map_y) * g->floor.height);
		/* clamp por si hay rounding negativo/extraño */
		ray->tx = clamp_int(ray->tx, 0, g->floor.width - 1);
		ray->ty = clamp_int(ray->ty, 0, g->floor.height - 1);
		ray->color = *(unsigned int *)(g->floor.addr + ray->ty * g->floor.line_len
				+ ray->tx * (g->floor.bpp / 8));
		put_pixel(g, x, y, ray->color);
		/* techo espejo */
		ray->delta_dist_x = clamp_int(ray->tx, 0, g->ceiling.width - 1);
		ray->delta_dist_y = clamp_int(ray->ty, 0, g->ceiling.height - 1);
		ray->color = *(unsigned int *)(g->ceiling.addr + (int)ray->delta_dist_y
				* g->ceiling.line_len + (int)ray->delta_dist_x * (g->ceiling.bpp
					/ 8));
		put_pixel(g, x, HEIGHT - y - 1, ray->color);
		ray->side_dist_x += ray->step_x;
		ray->side_dist_y += ray->step_y;
	}
}

void	calculate_wall_stripe(t_game *g, t_ray *ray, t_tex *tex)
{
	double	wallX;

	ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
	ray->draw_start_y = -ray->line_height / 2 + HEIGHT / 2;
	ray->draw_end_y = ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_start_y < 0)
		ray->draw_start_y = 0;
	if (ray->draw_end_y >= HEIGHT)
		ray->draw_end_y = HEIGHT - 1;
	wallX = g->spider.x + ray->perp_wall_dist * ray->dir_x0;
	if (ray->side == 0)
		wallX = g->spider.y + ray->perp_wall_dist * ray->dir_y0;
	wallX -= floor(wallX);
	ray->tx = (int)(wallX * tex->width);
	ray->tx = clamp_int(ray->tx, 0, tex->width - 1);
	if ((ray->side == 0 && ray->dir_x0 > 0) || (ray->side == 1
			&& ray->dir_y0 < 0))
		ray->tx = tex->width - ray->tx - 1;
}

void	draw_wall_stripe(t_game *g, t_ray *ray, t_tex *tex, int x)
{
	int	d;
	int	y;

	calculate_wall_stripe(g, ray, tex);
	y = ray->draw_start_y - 1;
	d = 0;
	while (++y <= ray->draw_end_y)
	{
		d = y * 256 - HEIGHT * 128 + ray->line_height * 128;
		ray->ty = ((d * tex->height) / ray->line_height) / 256;
		ray->ty = clamp_int(ray->ty, 0, tex->height - 1);
		ray->color = *(unsigned int *)(tex->addr + ray->ty * tex->line_len + ray->tx
			* (tex->bpp / 8));
		put_pixel(g, x, y, ray->color);
	}
}

void	print_map(t_game *g)
{
	t_pos	p;

	for (p.y = 0; p.y < MAP_H; p.y++)
	{
		for (p.x = 0; p.x < MAP_W; p.x++)
		{
			printf("%c", g->map[p.y][p.x]);
		}
		printf("\n");
	}
	printf("\n");
}
