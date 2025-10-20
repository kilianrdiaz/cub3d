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

void	calculate_wall_stripe(t_game *g, t_ray *ray, t_tex tex, int side)
{
	ray->line_height = (int)(HEIGHT / ray->row_distance);
	ray->d_start.y = -ray->line_height / 2 + HEIGHT / 2;
	ray->d_end.y = ray->line_height / 2 + HEIGHT / 2;
	if (ray->d_start.y < 0)
		ray->d_start.y = 0;
	if (ray->d_end.y >= HEIGHT)
		ray->d_end.y = HEIGHT - 1;
	ray->view = g->spider.x + ray->row_distance * ray->left.x;
	if (!side)
		ray->view = g->spider.y + ray->row_distance * ray->left.y;
	ray->view -= floor(ray->view);
	ray->src.x = (int)(ray->view * tex.width);
	ray->src.x = clamp_int(ray->src.x, 0, tex.width - 1);
	if ((!side == 0 && ray->left.x > 0) || (side && ray->left.y < 0))
		ray->src.x = tex.width - ray->src.x - 1;
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
	ray.view = 2 * x / (double)GAME_WIDTH - 1;
	ray.left.x = g->spider.dir_x + g->spider.plane_x * ray.view;
	ray.left.y = g->spider.dir_y + g->spider.plane_y * ray.view;
	// Posición inicial en el mapa (celda del jugador)
	ray.src.x = (int)g->spider.x;
	ray.src.y = (int)g->spider.y;
	// Distancias que recorrerá el rayo para cruzar una celda en X e Y
	ray.delta_dist_x = fabs(1.0 / ray.left.x);
	if (ray.left.x == 0.0)
		ray.delta_dist_x = 1e30;
	ray.delta_dist_y = fabs(1.0 / ray.left.y);
	if (ray.left.y == 0.0)
		ray.delta_dist_y = 1e30;
	ray.row_distance = 0.0;
	ray.coords.x = 1;
	ray.side_dist_x = (ray.src.x + 1.0 - g->spider.x) * ray.delta_dist_x;
	ray.coords.y = 1;
	ray.side_dist_y = (ray.src.y + 1.0 - g->spider.y) * ray.delta_dist_y;
	return (ray);
}
