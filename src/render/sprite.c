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

void	draw_sprite(t_game *g, t_sprite *sp, t_ray ray, int stripe, t_tex *tex)
{
	int				d;
	int				y;

	if (sp->state == DEFUSED)
		return ;
	ray.tx = (int)(256 * (stripe - (-sp->width / 2 + sp->screen_x))
			* tex[sp->state].width / sp->width) / 256;
	y = ray.draw_start_y - 1;
	while (++y < ray.draw_end_y)
	{
		d = (y - ray.draw_start_y) * 256;
		ray.ty = ((d * tex[sp->state].height) / sp->height) / 256;
		if (ray.tx < 0 || ray.tx >= tex[sp->state].width || ray.ty < 0
			|| ray.ty >= tex[sp->state].height)
			continue ;
		ray.color = *(unsigned int *)(tex[sp->state].addr + ray.ty * tex[sp->state].line_len
				+ ray.tx * (tex[sp->state].bpp / 8));
		if ((ray.color & 0x00FFFFFF) != 0) // Transparencia
			put_pixel(g, stripe, y, ray.color);
	}
}

static void	ray_sprite(t_sprite *sp, t_ray *ray, t_tex *tex)
{
	// 1️⃣ Altura y ancho del sprite según la distancia (como las paredes)
	sp->height = (tex[sp->state].height * (SCALE_SPRITE + sp->over_scale) / sp->trans_y);
	sp->width = (tex[sp->state].width * (SCALE_SPRITE + sp->over_scale) / sp->trans_y);
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
	sp->screen_x = (int)((GAME_WIDTH / 2) * (1 + sp->trans_x / sp->trans_y));
	ray->draw_start_x = -sp->width / 2 + sp->screen_x;
	ray->draw_end_x = ray->draw_start_x + sp->width;
	if (ray->draw_start_x < 0)
		ray->draw_start_x = 0;
	if (ray->draw_end_x >= GAME_WIDTH)
		ray->draw_end_x = GAME_WIDTH - 1;
}

void	position_sprite(t_game *g, t_sprite sp, t_tex *tex)
{
	t_ray		ray;
	int			stripe;

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
		ray_sprite(&sp, &ray, tex);
		// Dibujar sprite con z-buffe
		stripe = ray.draw_start_x - 1;
		while (++stripe < ray.draw_end_x)
			if (sp.trans_y > 0 && stripe >= 0 && stripe < GAME_WIDTH
				&& sp.trans_y < g->zbuffer[stripe])
				draw_sprite(g, &sp, ray, stripe, tex);
	}
}

void render_sprites(t_game *g, t_sprite **sprites, t_tex *tex)
{
	t_sprite_order	*order;
	int			i;
	int			count;

	if (!sprites)
		return ;
	i = -1;
	count = ft_memlen((void **)sprites);
	order = malloc(sizeof(t_sprite_order) * count);
	if (!order)
		ft_error_exit("Error: Memory allocation failed for sprite order\n");
	while (sprites[++i])
	{
		order[i].index = i;
		order[i].dist = pow(g->spider.x - sprites[i]->x, 2)
			+ pow(g->spider.y - sprites[i]->y, 2);
	}
	sort_sprites(order, count);
	i = -1;
	while (++i < count)
		position_sprite(g, *sprites[order[i].index], tex);
	free(order);
}
