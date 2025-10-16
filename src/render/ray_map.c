/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:45:16 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/16 15:45:09 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_tex	get_texture_wall(t_game g, t_ray ray)
{
	if (ray.side == 0)
	{
		if (ray.dir_x0 > 0)
			return (g.wall_west);
		return (g.wall_east);
	}
	else
	{
		if (ray.dir_y0 > 0)
			return (g.wall_south);
		return (g.wall_north);
	}
}

void	calculate_wall_stripe(t_game *g, t_ray *ray, t_tex tex)
{
	ray->line_height = (int)(HEIGHT / ray->perp_wall_dist);
	ray->draw_start_y = -ray->line_height / 2 + HEIGHT / 2;
	ray->draw_end_y = ray->line_height / 2 + HEIGHT / 2;
	if (ray->draw_start_y < 0)
		ray->draw_start_y = 0;
	if (ray->draw_end_y >= HEIGHT)
		ray->draw_end_y = HEIGHT - 1;
	ray->camera_x = g->spider.x + ray->perp_wall_dist * ray->dir_x0;
	if (ray->side == 0)
		ray->camera_x = g->spider.y + ray->perp_wall_dist * ray->dir_y0;
	ray->camera_x -= floor(ray->camera_x);
	ray->tx = (int)(ray->camera_x * tex.width);
	ray->tx = clamp_int(ray->tx, 0, tex.width - 1);
	if ((ray->side == 0 && ray->dir_x0 > 0) || (ray->side == 1
			&& ray->dir_y0 < 0))
		ray->tx = tex.width - ray->tx - 1;
}

void	print_map(t_game *g)
{
	t_pos	p;

	p.y = -1;
	while (g->map[++p.y])
	{
		p.x = -1;
		while (g->map[p.y][++p.x])
			printf("%c", g->map[p.y][p.x]);
		printf("\n");
	}
	printf("\n");
}

t_ray	ray_map(t_game *g, int x)
{
	t_ray	ray;

	ft_bzero(&ray, sizeof(t_ray));
	ray.camera_x = 2 * x / (double)GAME_WIDTH - 1;
	ray.dir_x0 = g->spider.dir_x + g->spider.plane_x * ray.camera_x;
	ray.dir_y0 = g->spider.dir_y + g->spider.plane_y * ray.camera_x;
	// Posición inicial en el mapa (celda del jugador)
	ray.tx = (int)g->spider.x;
	ray.ty = (int)g->spider.y;
	// Distancias que recorrerá el rayo para cruzar una celda en X e Y
	ray.delta_dist_x = fabs(1.0 / ray.dir_x0);
	if (ray.dir_x0 == 0.0)
		ray.delta_dist_x = 1e30;
	ray.delta_dist_y = fabs(1.0 / ray.dir_y0);
	if (ray.dir_y0 == 0.0)
		ray.delta_dist_y = 1e30;
	ray.perp_wall_dist = 0.0;
	ray.step_x = 1;
	ray.side_dist_x = (ray.tx + 1.0 - g->spider.x) * ray.delta_dist_x;
	ray.step_y = 1;
	ray.side_dist_y = (ray.ty + 1.0 - g->spider.y) * ray.delta_dist_y;
	return (ray);
}
