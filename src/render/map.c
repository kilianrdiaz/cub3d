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

char		*map[] = {"11111111111111111111", "1E000000000000000001",
			"11110111011111111001", "10000001000000001001",
			"10111111101111111001", "10000000000000000001",
			"10111111111111111111", "11111111111111111111"};

static void	paint(t_game *g, t_ray *ray, int y)
{
	int	x;
	int	tx;
	int	ty;
	int	color;

	x = -1;
	while (++x < WIDTH)
	{
		ray->mapX = (int)ray->sideDistX;
		ray->mapY = (int)ray->sideDistY;
		/* seguridad por si las texturas tienen tamaño 0 (ya comprobado al cargar) */
		tx = (int)((ray->sideDistX - ray->mapX) * g->floor.width);
		ty = (int)((ray->sideDistY - ray->mapY) * g->floor.height);
		/* clamp por si hay rounding negativo/extraño */
		tx = clamp_int(tx, 0, g->floor.width - 1);
		ty = clamp_int(ty, 0, g->floor.height - 1);
		color = *(unsigned int *)(g->floor.addr + ty * g->floor.line_len + tx
				* (g->floor.bpp / 8));
		put_pixel(g, x, y, color);
		/* techo espejo */
		ray->deltaDistX = clamp_int(tx, 0, g->ceiling.width - 1);
		ray->deltaDistY = clamp_int(ty, 0, g->ceiling.height - 1);
		color = *(unsigned int *)(g->ceiling.addr + (int)ray->deltaDistY
				* g->ceiling.line_len + (int)ray->deltaDistX * (g->ceiling.bpp
					/ 8));
		put_pixel(g, x, HEIGHT - y - 1, color);
		ray->sideDistX += ray->stepX;
		ray->sideDistY += ray->stepY;
	}
}

void	draw_floor_and_ceiling(t_game *g)
{
	t_ray	ray;
	int		y;
	int		p;

	y = HEIGHT / 2;
	while (++y < HEIGHT)
	{
		ray.dirX0 = g->dirX - g->planeX;
		ray.dirY0 = g->dirY - g->planeY;
		ray.dirX1 = g->dirX + g->planeX;
		ray.dirY1 = g->dirY + g->planeY;
		p = y - HEIGHT / 2;
		if (p == 0)
			continue ; /* seguro, aunque no debería pasar */
		ray.posZ = 0.5 * HEIGHT;
		ray.rowDistance = ray.posZ / (double)p;
		ray.sideDistX = g->posX + ray.rowDistance * ray.dirX0;
		ray.sideDistY = g->posY + ray.rowDistance * ray.dirY0;
		ray.stepX = ray.rowDistance * (ray.dirX1 - ray.dirX0) / (double)WIDTH;
		ray.stepY = ray.rowDistance * (ray.dirY1 - ray.dirY0) / (double)WIDTH;
        paint(g, &ray, y);
	}
}
