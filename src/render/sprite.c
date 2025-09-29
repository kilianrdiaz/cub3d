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
	t_pos			pos;

	pos.x = -1;
	while (++pos.x < count)
	{
		pos.y = pos.x;
		while (++pos.y < count)
		{
			if (order[pos.x].dist < order[pos.y].dist)
			{
				tmp = order[pos.x];
				order[pos.x] = order[pos.y];
				order[pos.y] = tmp;
			}
		}
	}
}

void	draw_sprite(t_game *g, t_sprite *sp, t_ray ray, int stripe)
{
	int				d;
	unsigned int	color;
	int				y;

	if (sp->state == DEFUSED)
		return ;
	ray.tx = (int)(256 * (stripe - (-sp->width / 2 + sp->screen_x))
			* sp->tex.width / sp->width) / 256;
	y = ray.draw_start_y - 1;
	while (++y < ray.draw_end_y)
	{
		d = (y - ray.draw_start_y) * 256;
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
	sp->height = abs((int)(BOMB_H / sp->trans_y));
	sp->width = abs((int)(sp->height * ((double)sp->tex.width
					/ sp->tex.height)));
	// 2️⃣ Offset vertical para apoyarlo en el suelo
	ray->camera_x = (int)(HEIGHT / sp->trans_y * 0.5);
	// 3️⃣ Límites verticales
	ray->draw_start_y = -sp->height / 2 + HEIGHT / 2 + ray->camera_x;
	ray->draw_end_y = sp->height / 2 + HEIGHT / 2 + ray->camera_x;
	if (ray->draw_start_y < 0)
		ray->draw_start_y = 0;
	if (ray->draw_end_y >= HEIGHT)
		ray->draw_end_y = HEIGHT - 1;
	// 4️⃣ Límites horizontales
	sp->screen_x = (int)((WIDTH / 2) * (1 + sp->trans_x / sp->trans_y));
	ray->draw_start_x = -sp->width / 2 + sp->screen_x;
	ray->draw_end_x = ray->draw_start_x + sp->width;
	if (ray->draw_start_x < 0)
		ray->draw_start_x = 0;
	if (ray->draw_end_x >= WIDTH)
		ray->draw_end_x = WIDTH - 1;
}

static void	position_sprite(t_game *g, t_sprite_order *order, int x)
{
	t_sprite	sp;
	t_ray		ray;
	int			stripe;

	sp = *g->bombs[order[x].index];
	sp.x = sp.x + 0.5 - g->spider.x;
	sp.y = sp.y + 0.5 - g->spider.y;
	// Transformar a espacio de cámara
	sp.inv_det = 1.0 / (g->spider.plane_x * g->spider.dir_y - g->spider.dir_x
			* g->spider.plane_y);
	sp.trans_x = sp.inv_det * (g->spider.dir_y * sp.x - g->spider.dir_x * sp.y);
	sp.trans_y = sp.inv_det * (-g->spider.plane_y * sp.x + g->spider.plane_x
			* sp.y);
	if (sp.trans_y > 0.0)
	{
		ray_sprite(&sp, &ray);
		// Dibujar sprite con z-buffe
		stripe = ray.draw_start_x - 1;
		while (++stripe < ray.draw_end_x)
			if (sp.trans_y > 0 && stripe >= 0 && stripe < WIDTH
				&& sp.trans_y < g->zbuffer[stripe])
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
				order[count].dist = pow(g->spider.x - g->bombs[idx]->x, 2)
					+ pow(g->spider.y - g->bombs[idx]->y, 2);
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
