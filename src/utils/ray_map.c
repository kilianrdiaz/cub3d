/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 14:45:16 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/24 19:54:13 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

t_tex	get_texture_wall(t_game g, t_ray ray, int side)
{
	if (!side)
	{
		if (ray.left.x > 0)
			return (g.map_text[WE]);
		return (g.map_text[EA]);
	}
	else
	{
		if (ray.left.y > 0)
			return (g.map_text[SO]);
		return (g.map_text[NO]);
	}
}

int	clamp_int(int v, int a, int b)
{
	if (v < a)
		return (a);
	if (v > b)
		return (b);
	return (v);
}

void	calculate_wall_stripe(t_game g, t_ray *ray, t_tex tex, int side)
{
	ray->line_height = (int)(HEIGHT / ray->row_distance);
	ray->d_start.y = -ray->line_height / 2 + HEIGHT / 2;
	ray->d_end.y = ray->line_height / 2 + HEIGHT / 2;
	if (ray->d_start.y < 0)
		ray->d_start.y = 0;
	if (ray->d_end.y >= HEIGHT)
		ray->d_end.y = HEIGHT - 1;
	ray->view = g.spider.pos.x + ray->row_distance * ray->left.x;
	if (!side)
		ray->view = g.spider.pos.y + ray->row_distance * ray->left.y;
	ray->view -= floor(ray->view);
	ray->src.x = (int)(ray->view * tex.width);
	ray->src.x = clamp_int(ray->src.x, 0, tex.width - 1);
	if ((!side && ray->left.x > 0) || (side && ray->left.y < 0))
		ray->src.x = tex.width - ray->src.x - 1;
}

void	calculate_distance_to_wall(t_game g, t_ray *ray, int *side)
{
	while (ray->src.x >= 0 && ray->src.y >= 0 && g.map[ray->src.y]
		&& g.map[ray->src.y][ray->src.x] != '1')
	{
		if (ray->side_dist.x < ray->side_dist.y)
		{
			ray->side_dist.x += ray->delta_dist.x;
			ray->src.x += ray->coords.x;
			*side = 0;
		}
		else
		{
			ray->side_dist.y += ray->delta_dist.y;
			ray->src.y += ray->coords.y;
			*side = 1;
		}
	}
	ray->row_distance = (ray->src.y - g.spider.pos.y + (1 - ray->coords.y)
			/ 2.0) / ray->left.y;
	if (!(*side))
		ray->row_distance = (ray->src.x - g.spider.pos.x + (1 - ray->coords.x)
				/ 2.0) / ray->left.x;
	if (ray->row_distance <= 0.0)
		ray->row_distance = 1e-6;
}

t_ray	ray_map(t_game g, int x)
{
	t_ray	ray;

	ft_bzero(&ray, sizeof(t_ray));
	ray.view = 2 * x / (double)GAME_W - 1;
	ray.left.x = g.spider.dir.x + g.spider.plane.x * ray.view;
	ray.left.y = g.spider.dir.y + g.spider.plane.y * ray.view;
	ray.src.x = (int)g.spider.pos.x;
	ray.src.y = (int)g.spider.pos.y;
	ray.delta_dist.x = fabs(1.0 / ray.left.x);
	if (ray.left.x == 0.0)
		ray.delta_dist.x = 1e30;
	ray.delta_dist.y = fabs(1.0 / ray.left.y);
	if (ray.left.y == 0.0)
		ray.delta_dist.y = 1e30;
	ray.row_distance = 0.0;
	ray.coords.x = 1;
	ray.side_dist.x = (ray.src.x + 1.0 - g.spider.pos.x) * ray.delta_dist.x;
	ray.coords.y = 1;
	ray.side_dist.y = (ray.src.y + 1.0 - g.spider.pos.y) * ray.delta_dist.y;
	return (ray);
}
