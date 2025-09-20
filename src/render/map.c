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
	while (++x < WIDTH)
	{
		ray->mapX = (int)ray->sideDistX;
		ray->mapY = (int)ray->sideDistY;
		/* seguridad por si las texturas tienen tamaño 0 (ya comprobado al cargar) */
		ray->tx = (int)((ray->sideDistX - ray->mapX) * g->floor.width);
		ray->ty = (int)((ray->sideDistY - ray->mapY) * g->floor.height);
		/* clamp por si hay rounding negativo/extraño */
		ray->tx = clamp_int(ray->tx, 0, g->floor.width - 1);
		ray->ty = clamp_int(ray->ty, 0, g->floor.height - 1);
		ray->color = *(unsigned int *)(g->floor.addr + ray->ty
				* g->floor.line_len + ray->tx * (g->floor.bpp / 8));
		put_pixel(g, x, y, ray->color);
		/* techo espejo */
		ray->deltaDistX = clamp_int(ray->tx, 0, g->ceiling.width - 1);
		ray->deltaDistY = clamp_int(ray->ty, 0, g->ceiling.height - 1);
		ray->color = *(unsigned int *)(g->ceiling.addr + (int)ray->deltaDistY
				* g->ceiling.line_len + (int)ray->deltaDistX * (g->ceiling.bpp
					/ 8));
		put_pixel(g, x, HEIGHT - y - 1, ray->color);
		ray->sideDistX += ray->stepX;
		ray->sideDistY += ray->stepY;
	}
}

void	calculate_wall_stripe(t_game *g, t_ray *ray, t_tex *tex)
{
	double	wallX;

	ray->lineHeight = (int)(HEIGHT / ray->perpWallDist);
	ray->drawStart = -ray->lineHeight / 2 + HEIGHT / 2;
	ray->drawEnd = ray->lineHeight / 2 + HEIGHT / 2;
	if (ray->drawStart < 0)
		ray->drawStart = 0;
	if (ray->drawEnd >= HEIGHT)
		ray->drawEnd = HEIGHT - 1;
	wallX = g->spider.posX + ray->perpWallDist * ray->dirX0;
	if (ray->side == 0)
		wallX = g->spider.posY + ray->perpWallDist * ray->dirY0;
	wallX -= floor(wallX);
	ray->tx = (int)(wallX * tex->width);
	ray->tx = clamp_int(ray->tx, 0, tex->width - 1);
	if ((ray->side == 0 && ray->dirX0 > 0) || (ray->side == 1
			&& ray->dirY0 < 0))
		ray->tx = tex->width - ray->tx - 1;
}

void	draw_wall_stripe(t_game *g, t_ray *ray, t_tex *tex, int x)
{
	int	d;
	int	y;

	calculate_wall_stripe(g, ray, tex);
	y = ray->drawStart - 1;
	d = 0;
	while (++y <= ray->drawEnd)
	{
		d = y * 256 - HEIGHT * 128 + ray->lineHeight * 128;
		ray->ty = ((d * tex->height) / ray->lineHeight) / 256;
		ray->ty = clamp_int(ray->ty, 0, tex->height - 1);
		ray->color = *(unsigned int *)(tex->addr + ray->ty * tex->line_len
				+ ray->tx * (tex->bpp / 8));
		put_pixel(g, x, y, ray->color);
	}
}

void	print_map(t_game *g)
{
	t_pos p;

	for (p.y = 0; p.y < MAP_H; p.y++)
	{
		for (p.x = 0; p.x < MAP_W; p.x++)
		{
			if (p.x == (int)g->spider.posX && p.y == (int)g->spider.posY)
				printf("P"); // posición del jugador
			else
				printf("%c", map[p.y][p.x]);
		}
		printf("\n");
	}
	printf("\n");
}
