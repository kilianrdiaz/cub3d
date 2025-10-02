/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:22 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 13:56:26 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	calculate_distance_to_wall(t_game *g, t_ray *ray)
{
	// 3. Algoritmo DDA para avanzar en el mapa hasta encontrar pared
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		// Protección: si se sale del mapa, forzamos colisión
		if (ray->map_x < 0 || ray->map_x >= MAP_W || ray->map_y < 0
			|| ray->map_y >= MAP_H || map[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
	// 4. Cálculo de la distancia perpendicular a la pared
	ray->perp_wall_dist = (ray->map_y - g->spider.y + (1 - ray->step_y) / 2.0)
		/ ray->dir_y0;
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - g->spider.x + (1 - ray->step_x)
				/ 2.0) / ray->dir_x0;
	if (ray->perp_wall_dist <= 0.0)
		ray->perp_wall_dist = 1e-6; // Evita divisiones por 0
}

static t_ray	initialize_ray(t_game *g, int x)
{
	t_ray	ray;

	ray.camera_x = 2 * x / (double)GAME_WIDTH - 1;
	ray.dir_x0 = g->spider.dir_x + g->spider.plane_x * ray.camera_x;
	ray.dir_y0 = g->spider.dir_y + g->spider.plane_y * ray.camera_x;
	// Posición inicial en el mapa (celda del jugador)
	ray.map_x = (int)g->spider.x;
	ray.map_y = (int)g->spider.y;
	// Distancias que recorrerá el rayo para cruzar una celda en X e Y
	ray.delta_dist_x = fabs(1.0 / ray.dir_x0);
	if (ray.dir_x0 == 0.0)
		ray.delta_dist_x = 1e30;
	ray.delta_dist_y = fabs(1.0 / ray.dir_y0);
	if (ray.dir_y0 == 0.0)
		ray.delta_dist_y = 1e30;
	ray.perp_wall_dist = 0.0;
	ray.hit = 0;// Flag: aún no ha golpeado pared
	ray.side = 0; // Flag: 0 = golpe en eje X, 1 = golpe en eje Y
	ray.step_x = 1;
	ray.side_dist_x = (ray.map_x + 1.0 - g->spider.x) * ray.delta_dist_x;
	ray.step_y = 1;
	ray.side_dist_y = (ray.map_y + 1.0 - g->spider.y) * ray.delta_dist_y;
	return (ray);
}

static void	render_wall(t_game *g, int x)
{
	t_ray	ray;
	t_tex	*tex;

	ray = initialize_ray(g, x);
	if (ray.dir_x0 < 0)
	{
		ray.step_x = -1;
		ray.side_dist_x = (g->spider.x - ray.map_x) * ray.delta_dist_x;
	}
	if (ray.dir_y0 < 0)
	{
		ray.step_y = -1;
		ray.side_dist_y = (g->spider.y - ray.map_y) * ray.delta_dist_y;
	}
	calculate_distance_to_wall(g, &ray);
	tex = get_texture_wall(g, &ray);
	draw_wall_stripe(g, &ray, tex, x);
	g->zbuffer[x] = ray.perp_wall_dist; // Guardamos la distancia del rayo
}

static void	render_floor_and_ceiling(t_game *g)
{
	t_ray	ray;
	int		y;
	int		p;

	y = HEIGHT / 2;
	while (++y < HEIGHT)
	{
		ray.dir_x0 = g->spider.dir_x - g->spider.plane_x;
		ray.dir_y0 = g->spider.dir_y - g->spider.plane_y;
		ray.dir_x1 = g->spider.dir_x + g->spider.plane_x;
		ray.dir_y1 = g->spider.dir_y + g->spider.plane_y;
		p = y - HEIGHT / 2;
		if (p == 0)
			continue ;
		ray.pos_z = 0.5 * HEIGHT;
		ray.row_distance = ray.pos_z / (double)p;
		ray.side_dist_x = g->spider.x + ray.row_distance * ray.dir_x0;
		ray.side_dist_y = g->spider.y + ray.row_distance * ray.dir_y0;
		ray.step_x = ray.row_distance * (ray.dir_x1 - ray.dir_x0)
			/ (double)GAME_WIDTH;
		ray.step_y = ray.row_distance * (ray.dir_y1 - ray.dir_y0)
			/ (double)GAME_WIDTH;
		draw_floor_and_ceiling(g, &ray, y);
	}
}

int	render(t_game *g)
{
	int	x;

	update_player_position(g); // Actualiza posición según teclas
	clean_screen(g);
	render_floor_and_ceiling(g);
	x = -1;
	while (++x < GAME_WIDTH)
		render_wall(g, x);
	render_sprites(g); // Dibuja las bombas
	update_bombs(g); // Actualiza estado de las bombas
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	draw_hand(g);
	if (g->spider.state == ATTACKING)
		spider_attack(g);
	return (0);
}
