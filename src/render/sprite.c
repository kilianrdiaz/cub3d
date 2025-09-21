/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:22:09 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/19 16:22:11 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	sort_sprites(t_sprite_order *order, int count)
{
	t_sprite_order	tmp;

	int i, j;
	for (i = 0; i < count - 1; i++)
	{
		for (j = i + 1; j < count; j++)
		{
			if (order[i].dist < order[j].dist)
			{
				tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
			}
		}
	}
}

void	draw_sprite(t_game *g, t_sprite *sp, t_ray ray, int stripe)
{
	int				d;
	unsigned int	color;
	int				y;

	ray.tx = (int)(256 * (stripe - (-sp->width / 2 + sp->screenX))
			* sp->tex.width / sp->width) / 256;
	y = ray.drawStartY - 1;
	while (++y < ray.drawEndY)
	{
		d = (y - ray.drawStartY) * 256;
		ray.ty = ((d * sp->tex.height) / sp->height) / 256;
		if (ray.tx < 0 || ray.tx >= sp->tex.width || ray.ty < 0
			|| ray.ty >= sp->tex.height)
			continue ;
		color = *(unsigned int *)(sp->tex.addr + ray.ty * sp->tex.line_len
				+ ray.tx * (sp->tex.bpp / 8));
		if ((color & 0x00FFFFFF) != 0) // Transparencia
			put_pixel(g, stripe, y, color);
	}
}

static void	ray_sprite(t_sprite *sp, t_ray *ray)
{
	// 1️⃣ Altura y ancho del sprite según la distancia (como las paredes)
	sp->height = abs((int)(BOMB_H / sp->transformY));
	sp->width = abs((int)(sp->height * ((double)sp->tex.width
					/ sp->tex.height)));
	// 2️⃣ Offset vertical para apoyarlo en el suelo
	ray->cameraX = (int)(HEIGHT / sp->transformY * 0.5);
	// 3️⃣ Límites verticales
	ray->drawStartY = -sp->height / 2 + HEIGHT / 2 + ray->cameraX;
	ray->drawEndY = sp->height / 2 + HEIGHT / 2 + ray->cameraX;
	if (ray->drawStartY < 0)
		ray->drawStartY = 0;
	if (ray->drawEndY >= HEIGHT)
		ray->drawEndY = HEIGHT - 1;
	// 4️⃣ Límites horizontales
	sp->screenX = (int)((WIDTH / 2) * (1 + sp->transformX / sp->transformY));
	ray->drawStartX = -sp->width / 2 + sp->screenX;
	ray->drawEndX = ray->drawStartX + sp->width;
	if (ray->drawStartX < 0)
		ray->drawStartX = 0;
	if (ray->drawEndX >= WIDTH)
		ray->drawEndX = WIDTH - 1;
}

static void	position_sprite(t_game *g, t_sprite_order *order, int x)
{
	t_sprite	sp;
	t_ray		ray;
	int			stripe;

	sp = *g->bombs[order[x].index];
	sp.spriteX = sp.x + 0.5 - g->spider.posX;
	sp.spriteY = sp.y + 0.5 - g->spider.posY;
	// Transformar a espacio de cámara
	sp.invDet = 1.0 / (g->spider.planeX * g->spider.dirY - g->spider.dirX
			* g->spider.planeY);
	sp.transformX = sp.invDet * (g->spider.dirY * sp.spriteX - g->spider.dirX
			* sp.spriteY);
	sp.transformY = sp.invDet * (-g->spider.planeY * sp.spriteX
			+ g->spider.planeX * sp.spriteY);
	if (sp.transformY > 0.0)
	{
		ray_sprite(&sp, &ray);
		// Dibujar sprite con z-buffe
		stripe = ray.drawStartX - 1;
		while (++stripe < ray.drawEndX)
			if (sp.transformY > 0 && stripe >= 0 && stripe < WIDTH
				&& sp.transformY < g->zbuffer[stripe])
				draw_sprite(g, &sp, ray, stripe);
	}
}

void	render_sprites(t_game *g)
{
	t_sprite_order	order[MAP_W * MAP_H];
	int				count;
	int				idx;
	t_pos			p;

	count = 0;
	p.y = -1;
	while (++p.y < MAP_H)
	{
		p.x = -1;
		while (++p.x < MAP_W)
		{
			idx = p.x + p.y * MAP_W;
			if (g->bombs[idx] != NULL)
			{
				order[count].index = idx;
				order[count].dist = pow(g->spider.posX - g->bombs[idx]->x, 2)
					+ pow(g->spider.posY - g->bombs[idx]->y, 2);
				// distancia al jugador
				count++;
			}
		}
	}
	sort_sprites(order, count);
	p.x = -1;
	while (++p.x < count)
		position_sprite(g, order, p.x);
}
