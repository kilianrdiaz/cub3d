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
		if (ray->sideDistX < ray->sideDistY)
		{
			ray->sideDistX += ray->deltaDistX;
			ray->mapX += ray->stepX;
			ray->side = 0;
		}
		else
		{
			ray->sideDistY += ray->deltaDistY;
			ray->mapY += ray->stepY;
			ray->side = 1;
		}
		// Protección: si se sale del mapa, forzamos colisión
		if (ray->mapX < 0 || ray->mapX >= MAP_W || ray->mapY < 0
			|| ray->mapY >= MAP_H || map[ray->mapY][ray->mapX] == '1')
			ray->hit = 1;
	}
	// 4. Cálculo de la distancia perpendicular a la pared
	ray->perpWallDist = (ray->mapY - g->spider.posY + (1 - ray->stepY) / 2.0)
		/ ray->dirY0;
	if (ray->side == 0)
		ray->perpWallDist = (ray->mapX - g->spider.posX + (1 - ray->stepX)
				/ 2.0) / ray->dirX0;
	if (ray->perpWallDist <= 0.0)
		ray->perpWallDist = 1e-6; // Evita divisiones por 0
}

static t_ray	initialize_ray(t_game *g, int x)
{
	t_ray	ray;

	ray.cameraX = 2 * x / (double)WIDTH - 1;
	ray.dirX0 = g->spider.dirX + g->spider.planeX * ray.cameraX;
	ray.dirY0 = g->spider.dirY + g->spider.planeY * ray.cameraX;
	// Posición inicial en el mapa (celda del jugador)
	ray.mapX = (int)g->spider.posX;
	ray.mapY = (int)g->spider.posY;
	// Distancias que recorrerá el rayo para cruzar una celda en X e Y
	ray.deltaDistX = fabs(1.0 / ray.dirX0);
	if (ray.dirX0 == 0.0)
		ray.deltaDistX = 1e30;
	ray.deltaDistY = fabs(1.0 / ray.dirY0);
	if (ray.dirY0 == 0.0)
		ray.deltaDistY = 1e30;
	ray.perpWallDist = 0.0;
	ray.hit = 0;  // Flag: aún no ha golpeado pared
	ray.side = 0; // Flag: 0 = golpe en eje X, 1 = golpe en eje Y
	ray.stepX = 1;
	ray.sideDistX = (ray.mapX + 1.0 - g->spider.posX) * ray.deltaDistX;
	ray.stepY = 1;
	ray.sideDistY = (ray.mapY + 1.0 - g->spider.posY) * ray.deltaDistY;
	return (ray);
}

static void	render_wall(t_game *g, int x)
{
	t_ray	ray;
	t_tex	*tex;

	ray = initialize_ray(g, x);
	if (ray.dirX0 < 0)
	{
		ray.stepX = -1;
		ray.sideDistX = (g->spider.posX - ray.mapX) * ray.deltaDistX;
	}
	if (ray.dirY0 < 0)
	{
		ray.stepY = -1;
		ray.sideDistY = (g->spider.posY - ray.mapY) * ray.deltaDistY;
	}
	calculate_distance_to_wall(g, &ray);
	tex = get_texture_wall(g, &ray);
	draw_wall_stripe(g, &ray, tex, x);
}

int	render(t_game *g)
{
	int	x;

	usleep(200000);
	update_player_position(g); // Actualiza posición según teclas
	clean_screen(g);
	draw_floor_and_ceiling(g);
	x = -1;
	while (++x < WIDTH)
		render_wall(g, x);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	draw_hand(g);
	// printf("Nueva pos: (%f, %f)\n", g->spider.posX, g->spider.posY);
	return (0);
}
