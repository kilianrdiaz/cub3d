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

int	render(t_game *g)
{
	t_ray	ray;
	int		x;
	int		lineHeight;
	int		drawStart;
	int		drawEnd;
	double	wallX;
	t_tex	*tex;
	int		texX;
	int		d;
	int		texY;
	int		color;

	clean_screen(g);
	draw_floor_and_ceiling(g);
	x = -1;
	while (++x < WIDTH)
	{
		ray.cameraX = 2 * x / (double)WIDTH - 1;
		ray.dirX0 = g->dirX + g->planeX * ray.cameraX;
		ray.dirY0 = g->dirY + g->planeY * ray.cameraX;
		ray.mapX = (int)g->posX;
		ray.mapY = (int)g->posY;
		ray.deltaDistX = (ray.dirX0 == 0.0) ? 1e30 : fabs(1.0 / ray.dirX0);
		ray.deltaDistY = (ray.dirY0 == 0.0) ? 1e30 : fabs(1.0 / ray.dirY0);
		ray.perpWallDist = 0.0;
		ray.hit = 0;
		ray.side = 0;
		if (ray.dirX0 < 0)
		{
			ray.stepX = -1;
			ray.sideDistX = (g->posX - ray.mapX) * ray.deltaDistX;
		}
		else
		{
			ray.stepX = 1;
			ray.sideDistX = (ray.mapX + 1.0 - g->posX) * ray.deltaDistX;
		}
		if (ray.dirY0 < 0)
		{
			ray.stepY = -1;
			ray.sideDistY = (g->posY - ray.mapY) * ray.deltaDistY;
		}
		else
		{
			ray.stepY = 1;
			ray.sideDistY = (ray.mapY + 1.0 - g->posY) * ray.deltaDistY;
		}
		while (ray.hit == 0)
		{
			if (ray.sideDistX < ray.sideDistY)
			{
				ray.sideDistX += ray.deltaDistX;
				ray.mapX += ray.stepX;
				ray.side = 0;
			}
			else
			{
				ray.sideDistY += ray.deltaDistY;
				ray.mapY += ray.stepY;
				ray.side = 1;
			}
			/* PROTECCIÓN: fuera del mapa -> tratamos como hit y salimos */
			if (ray.mapX < 0 || ray.mapX >= MAP_W || ray.mapY < 0
				|| ray.mapY >= MAP_H)
			{
				ray.hit = 1;
				break ;
			}
			if (map[ray.mapY][ray.mapX] == '1')
				ray.hit = 1;
		}
		if (ray.side == 0)
			ray.perpWallDist = (ray.dirX0 == 0.0) ? 1e-6 : (ray.mapX - g->posX
					+ (1 - ray.stepX) / 2.0) / ray.dirX0;
		else
			ray.perpWallDist = (ray.dirY0 == 0.0) ? 1e-6 : (ray.mapY - g->posY
					+ (1 - ray.stepY) / 2.0) / ray.dirY0;
		if (ray.perpWallDist <= 0.0)
			ray.perpWallDist = 1e-6; /* protección extra */
		lineHeight = (int)(HEIGHT / ray.perpWallDist);
		drawStart = -lineHeight / 2 + HEIGHT / 2;
		drawEnd = lineHeight / 2 + HEIGHT / 2;
		if (drawStart < 0)
			drawStart = 0;
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;
		wallX = (ray.side == 0) ? g->posY + ray.perpWallDist
			* ray.dirY0 : g->posX + ray.perpWallDist * ray.dirX0;
		wallX -= floor(wallX);
		if (ray.side == 0)
		{
			if (ray.dirX0 > 0)
				tex = &g->wall_west;
			else
				tex = &g->wall_east;
		}
		else
		{
			if (ray.dirY0 > 0)
				tex = &g->wall_north;
			else
				tex = &g->wall_south;
		}
		texX = (int)(wallX * (double)tex->width);
		if (texX < 0)
			texX = 0;
		if (texX >= tex->width)
			texX = tex->width - 1;
		if ((ray.side == 0 && ray.dirX0 > 0) || (ray.side == 1
				&& ray.dirY0 < 0))
			texX = tex->width - texX - 1;
		for (int y = drawStart; y <= drawEnd; y++)
		{
			d = y * 256 - HEIGHT * 128 + lineHeight * 128;
			texY = ((d * tex->height) / lineHeight) / 256;
			texY = clamp_int(texY, 0, tex->height - 1);
			color = *(unsigned int *)(tex->addr + texY * tex->line_len + texX
					* (tex->bpp / 8));
			put_pixel(g, x, y, color);
		}
	}
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	draw_hand(g);
	return (0);
}

int	main(void)
{
	t_game	g;

	g.mlx = mlx_init();
	if (!g.mlx)
	{
		fprintf(stderr, "mlx_init failed\n");
		return (1);
	}
	g.win = mlx_new_window(g.mlx, WIDTH, HEIGHT, "Cub3D");
	if (!g.win)
	{
		fprintf(stderr, "mlx_new_window failed\n");
		return (1);
	}
	g.img = mlx_new_image(g.mlx, WIDTH, HEIGHT);
	if (!g.img)
	{
		fprintf(stderr, "mlx_new_image failed\n");
		return (1);
	}
	g.addr = mlx_get_data_addr(g.img, &g.bpp, &g.line_len, &g.endian);
	init_player(&g);
	/* Carga texturas: ajusta paths según tus archivos */
	load_texture(&g, &g.hand, "./textures/spiderhand_01.xpm");
	load_texture(&g, &g.floor, "./textures/floor.xpm");
	load_texture(&g, &g.ceiling, "./textures/ceiling.xpm");
	load_texture(&g, &g.wall_north, "./textures/wall_inner_left.xpm");
	load_texture(&g, &g.wall_south, "./textures/wall_inner_right.xpm");
	load_texture(&g, &g.wall_east, "./textures/wall_inner_right.xpm");
	load_texture(&g, &g.wall_west, "./textures/wall_inner_left.xpm");
	mlx_loop_hook(g.mlx, render, &g);
	mlx_loop(g.mlx);
	return (0);
}
